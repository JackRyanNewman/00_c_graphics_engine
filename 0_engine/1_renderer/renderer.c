/*┌─────────────────────────────────────────────────────────────────────────────┐
  │ CNN - Renderer                                                              │
  │   • © 2025 Jack Ryan Newman                                                 │
  ├─────────────────────────────────────────────────────────────────────────────┤
  └─────────────────────────────────────────────────────────────────────────────┘*/

/*=================================================================================================*/
/* Includes */
#include <stddef.h>
#include "renderer.h"
#include "../2_scene/scene.h"
#include "../3_camera/camera.h"
#include "../6_debugger/debugger.h"


/*=================================================================================================*/
/* Internal State */
static Rendered_Object* curr_scene; static int num_objs;    //Array of Rendered Objects
static Mesh** mesh_lst; static int num_meshes;             //Array holding all all meshes
static Material** mat_lst; static int num_mats;             //Array holding all materials

/*=================================================================================================*/
/* Renderer Lifecycle: Used by engine */

    bool renderer_load_scene(int scene_index) { 
        if (DEBUG_MODE) printf("    > renderer_load_scene started\n");
        camera_init((float)WINDOW_WIDTH/(float)WINDOW_HEIGHT);
        curr_scene = scene_1(&num_objs, &mesh_lst, &num_meshes, &mat_lst, &num_mats);
        for(int i=0; i < num_meshes; i++) val_crit_error_1(!geometry_create_mesh(mesh_lst[i]), 0, "Failed intilization of meshes.\n");
        for(int i=0; i < num_mats; i++) val_crit_error_1(!shader_create_program(mat_lst[i]), 0, "Failed intilization of shaders.\n");
        if (DEBUG_MODE) printf("    > Finished Geometeries and shaders compiliation\n");

        return 1;
    }

    bool geometry_create_mesh(Mesh* mesh) {
        // DON'T calculate - use the vertex_count and index_count from the mesh definition
        // size_t vertex_count = mesh->vertex_count;  // Already set in mesh definition
        // size_t index_count = mesh->index_count;    // Already set in mesh definition

        glGenVertexArrays(1, &mesh->vao);
        glGenBuffers(1, &mesh->vbo);
        glGenBuffers(1, &mesh->ebo);

        glBindVertexArray(mesh->vao);

        // Use vertex_count in floats * sizeof(float)
        glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
        glBufferData(GL_ARRAY_BUFFER, mesh->vertex_count * sizeof(float), mesh->vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index_count * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
        return 1;
    }

    bool shader_create_program(Material *material) {                                      
       
        //Compile the shaders
            int shaderSize = material->shader_count;  
            GLuint program = glCreateProgram();  // Create a new shader program object
            GLuint shaders[shaderSize];          // Tmp shader array.
        //Compiling Shaders
            for(int i=0; i< shaderSize; i++){
                GLuint shader = shader_compile(material->GL_types[i], material->shaders[i]);
                if(shader==0){
                    for(int a=0;a<i;a++){glDeleteShader(shaders[a]);}   
                    glDeleteProgram(program);
                    return 0;
                }  
                glAttachShader(program, shader); // Attach the compiled shader
                shaders[i] = shader;
            }
        //Check linking status and handle errors
            glLinkProgram(program); // Link the program (combine shaders into one GPU program)
            int success; glGetProgramiv(program, GL_LINK_STATUS, &success);     // Query whether linking succeeded
            if (!success) {                                                     // If linking failed:
                char log[512]; glGetProgramInfoLog(program, 512, NULL, log);      // Retrieve the linking error log
                for(int i=0; i< shaderSize; i++){ glDeleteShader(shaders[i]);}     
                glDeleteProgram(program);
                crit_error("Shader linking failed: %s\n", log);
            }
        
        //The program retains internal copies of compiled shader objects 
        //after successful linking; the standalone shader objects may be deleted.
        for(int i=0; i< shaderSize; i++){ glDeleteShader(shaders[i]);}
        material->material = program;
        return 1;
    }

    GLuint shader_compile(GLenum type, const char* source) {               
    // Compile a single shader of given type
        GLuint shader = glCreateShader(type);                               // Create shader object on GPU
        glShaderSource(shader, 1, &source, NULL);                           // Set the source code of the shader
        glCompileShader(shader);                                            // Compile the shader
        
        // Check compilation status
        int success; glGetShaderiv(shader, GL_COMPILE_STATUS, &success);    // Query whether compilation succeeded
        if (!success) {                                                     // If compilation failed:
            char log[512]; glGetShaderInfoLog(shader, 512, NULL, log);        // Retrieve the compilation error log
            glDeleteShader(shader);
            crit_error("Shader compilation failed: %s\n", log);
        }
        return shader;                                                      // Return compiled shader handle
    }

    void renderer_cleanup() {

        /*============================================================*/
        /* Delete mesh GPU resources */
        for (int i = 0; i < num_meshes; i++) {
            Mesh *m = mesh_lst[i];

            if (m->vao) glDeleteVertexArrays(1, &m->vao);
            if (m->vbo) glDeleteBuffers(1, &m->vbo);
            if (m->ebo) glDeleteBuffers(1, &m->ebo);

            m->vao = m->vbo = m->ebo = 0;
        }

        /*============================================================*/
        /* Delete shader programs */
        for (int i = 0; i < num_mats; i++) {
            Material *mat = mat_lst[i];

            if (mat->material) {
                glDeleteProgram(mat->material);
                mat->material = 0;
            }
        }

        /*============================================================*/
        /* Clear renderer-owned pointers (optional but clean) */
        curr_scene    = NULL;
        mesh_lst       = NULL;
        mat_lst    = NULL;

        num_objs = num_meshes = num_mats = 0;
    }

/*=================================================================================================*/
/* Renderer Lifecycle: Used by engine */
void renderer_render_frame() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    Mat4 projection = camera_get_projection_matrix();
    Mat4 view = camera_get_view_matrix();
    
    GLuint current_shader = 0;
    GLint view_loc, proj_loc, model_loc;
    
    for(int ob = 0; ob < num_objs; ob++){
        GLuint shader_program = curr_scene[ob].material->material;
        
        // Only switch shader and set view/projection if it changed
        if(shader_program != current_shader) {
            current_shader = shader_program;
            glUseProgram(shader_program);
            
            view_loc = glGetUniformLocation(shader_program, "view");
            proj_loc = glGetUniformLocation(shader_program, "projection");
            model_loc = glGetUniformLocation(shader_program, "model");
            
            // Set view and projection (same for all objects with this shader)
            glUniformMatrix4fv(view_loc, 1, GL_FALSE, view.m);
            glUniformMatrix4fv(proj_loc, 1, GL_FALSE, projection.m);
        }
        
        // Bind mesh and set model matrix (changes per object)
        glBindVertexArray(curr_scene[ob].parent_mesh->vao);
        Mat4 model = Transform_get_model_matrix(curr_scene[ob].transform);
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model.m);
        
        // Draw
        glDrawElements(curr_scene[ob].draw_mode, (GLsizei)curr_scene[ob].parent_mesh->index_count, GL_UNSIGNED_INT, 0);
    }
    
    glBindVertexArray(0);
}







