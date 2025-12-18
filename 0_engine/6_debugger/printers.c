/* Includes */
#include <stddef.h>
#include "../1_renderer/renderer.h"
#include "../2_scene/scene.h"
#include "../3_camera/camera.h"
#include "../6_debugger/debugger.h"


/*=================================================================================================*/
/* Internal State */
static Rendered_Object* curr_scene; static int num_objs;    //Array of Rendered Objects
static Mesh** mesh_lst; static int num_meshes;             //Array holding all all meshes
static Material** mat_lst; static int num_mats;             //Array holding all materials


const char* draw_mode_to_string(GLenum mode);
void debug_print_all_meshes(void);

void debug_print_mesh(Mesh* mesh);
void debug_print_scene(void);




const char* draw_mode_to_string(GLenum mode) {
    switch (mode) {
        case GL_TRIANGLES: return "GL_TRIANGLES";
        case GL_LINES:     return "GL_LINES";
        case GL_POINTS:   return "GL_POINTS";
        default:          return "UNKNOWN_DRAW_MODE";
    }
}

void debug_print_all_meshes(void) {
    printf("==== MESH REGISTRY (%d meshes) ====\n", num_meshes);
    for (int i = 0; i < num_meshes; i++) {
        printf("\nMesh [%d]\n", i);
        debug_print_mesh(mesh_lst[i]);
    }
}

void debug_print_mesh(Mesh* mesh) {
    if (!mesh) {
        printf("Mesh: NULL\n");
        return;
    }

    printf("===== MESH DEBUG =====\n");
    printf("VAO: %u  VBO: %u  EBO: %u\n",
           mesh->vao, mesh->vbo, mesh->ebo);

    printf("Vertex count (floats): %zu\n", mesh->vertex_count);
    printf("Index count: %zu\n", mesh->index_count);

    printf("\nVertices:\n");
    for (size_t i = 0; i < mesh->vertex_count; i += 6) {
        printf("  [%3zu] Pos(%.2f %.2f %.2f)  Col(%.2f %.2f %.2f)\n",
               i / 6,
               mesh->vertices[i + 0],
               mesh->vertices[i + 1],
               mesh->vertices[i + 2],
               mesh->vertices[i + 3],
               mesh->vertices[i + 4],
               mesh->vertices[i + 5]);
    }

    printf("\nIndices:\n");
    for (size_t i = 0; i < mesh->index_count; i += 3) {
        printf("  [%3zu] %u %u %u\n",
               i / 3,
               mesh->indices[i],
               mesh->indices[i + 1],
               mesh->indices[i + 2]);
    }

    printf("======================\n");
}

void debug_print_scene(void) {
    debug_print_all_meshes();
    if (!curr_scene || num_objs <= 0) {
        printf("Scene is empty.\n");
        return;
    }

    printf("===== SCENE DEBUG =====\n");
    printf("Total objects: %d\n\n", num_objs);

    for (int i = 0; i < num_objs; i++) {
        Rendered_Object* obj = &curr_scene[i];

        printf("Object %d:\n", i);

        printf("  Mesh:      %p\n", (void*)obj->parent_mesh);
        printf("  Material:  %p\n", (void*)obj->material);
        printf("  Draw mode: %s\n", draw_mode_to_string(obj->draw_mode));

        if (obj->transform) {
            Transform* t = obj->transform;
            printf("  Transform:\n");
            printf("    Position: (%.2f, %.2f, %.2f)\n",t->position.x, t->position.y, t->position.z);
            printf("    Rotation: (%.2f, %.2f, %.2f)\n", t->rotation.x, t->rotation.y, t->rotation.z);
            printf("    Scale:    (%.2f, %.2f, %.2f)\n", t->scale.x, t->scale.y, t->scale.z);
        } else {
            printf("  Transform: NULL\n");
        }

        printf("\n");
    }

    printf("=======================\n");
}
