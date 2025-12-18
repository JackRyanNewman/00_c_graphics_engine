/*┌─────────────────────────────────────────────────────────────────────────────┐
  │ CNN - Scene Management                                                      │
  │   • © 2025 Jack Ryan Newman                                                 │
  ├─────────────────────────────────────────────────────────────────────────────┤
  └─────────────────────────────────────────────────────────────────────────────┘*/

/*=================================================================================================*/
/* Includes */
#include <omp.h>
#include <stdio.h>
#include <stddef.h>
#include "..\2_scene\scene.h"
#include "..\2_scene\static_assets\Static_Attributes.h"
#include "..\2_scene\static_assets\Static_Materials.h"
#include "..\2_scene\static_assets\Static_Meshes.h"


/*=================================================================================================*/
/* Internal State */
static Vec3 base_rotation = {1,1,1}; 
Transform **Rotating_Objects; int total_rotating;  //Array of objects that move.
 

/*=================================================================================================*/
/* Scene Lifecycle */

void scene_update(float dt) {
    // Update all object rotations in parallel
    #pragma omp parallel for
    for(int i = 0; i < total_rotating; i++){
        Transform_update(Rotating_Objects[i], dt);
    } 
}

void Transform_update(Transform* obj, float dt) {
    // Update rotation based on rotation speed
    obj->rotation.x += base_rotation.x * dt; 
    obj->rotation.y += base_rotation.y * dt;
    obj->rotation.z += base_rotation.z * dt;
}

Mat4 Transform_get_model_matrix(Transform* obj) {
    // Only translastion and rotation - scale already baked into vertices
    Mat4 translation = mat4_translate(obj->position);
    Mat4 rot_x = mat4_rotate_x(obj->rotation.x);
    Mat4 rot_y = mat4_rotate_y(obj->rotation.y);
    Mat4 rot_z = mat4_rotate_z(obj->rotation.z);
    Mat4 rotation = mat4_multiply(rot_z, mat4_multiply(rot_y, rot_x));
    
    return mat4_multiply(translation, rotation);
}

void scene_cleanup() {}
//


/*=================================================================================================*/
/*Methods*/
Rendered_Object* scene_1(int* total_objs, Mesh*** mesh_lst, int* num_meshes, Material*** mat_lst, int* num_mats){
    // Static transforms (rotating objects)
    
    static Transform walls = { .actions=NULL, .position={0, 0, 0},  .rotation={0, 0, 0}, .scale={1, 1, 1} };
    static Transform obj_1 = { .actions=NULL, .position={2, 0, 0},  .rotation={0, 0, 0}, .scale={1, 1, 1} };
    static Transform obj_2 = { .actions=NULL, .position={-2, 0, 0}, .rotation={0, 0, 0}, .scale={1, 1, 1} };
    static Transform obj_3 = { .actions=NULL, .position={0, 2, 0},  .rotation={0, 0, 0}, .scale={1, 1, 1} };

    // All meshes used in the scene
    static Mesh* meshes[] = { &Room_Mesh, &Wire_Cube, &Full_Diamond, &Wire_Diamond };
    *num_meshes = sizeof(meshes)/sizeof(meshes[0]);
    *mesh_lst = meshes;  

    // All materials used in the scene
    static Material* materials[] = { &Base_Color };
    *num_mats = sizeof(materials)/sizeof(materials[0]);
    *mat_lst = materials; 

    // Scene objects
    static Rendered_Object scene[] = {
        { .parent_mesh = &Room_Mesh,      .material = &Base_Color, .transform = &walls, .draw_mode = GL_TRIANGLES },
        { .parent_mesh = &Wire_Cube,      .material = &Base_Color, .transform = &obj_1, .draw_mode = GL_LINES },
        { .parent_mesh = &Full_Diamond,   .material = &Base_Color, .transform = &obj_2, .draw_mode = GL_TRIANGLES },
        { .parent_mesh = &Wire_Diamond,   .material = &Base_Color, .transform = &obj_3, .draw_mode = GL_LINES }
    };
    *total_objs = sizeof(scene)/sizeof(scene[0]);
    apply_scale_to_meshes(scene, *total_objs, meshes, *num_meshes);
    static Transform* rotating[] = { &obj_1, &obj_2, &obj_3 };
    total_rotating = sizeof(rotating)/sizeof(rotating[0]);
    Rotating_Objects = rotating;

    return scene;

}

void apply_scale_to_meshes(Rendered_Object* scene, int num_objs, Mesh** meshes, int num_meshes) {
    // Track which meshes we've already scaled (avoid scaling the same mesh multiple times)
    int scaled[num_meshes];
    for (int i = 0; i < num_meshes; i++) scaled[i] = 0;
    
    // For each object in the scene
    for (int obj_idx = 0; obj_idx < num_objs; obj_idx++) {
        Rendered_Object* obj = &scene[obj_idx];
        Mesh* mesh = (Mesh*)obj->parent_mesh;
        Vec3 scale = obj->transform->scale;
        
        // Find which mesh index this is
        int mesh_idx = -1;
        for (int m = 0; m < num_meshes; m++) {
            if (meshes[m] == mesh) {
                mesh_idx = m;
                break;
            }
        }
        
        // Skip if we already scaled this mesh or if scale is default (1,1,1)
        if (mesh_idx == -1 || scaled[mesh_idx]) continue;
        if (scale.x == 1.0f && scale.y == 1.0f && scale.z == 1.0f) continue;
        
        // Apply scale to all vertices in this mesh
        for (size_t i = 0; i < mesh->vertex_count; i += 6) {
            mesh->vertices[i + 0] *= scale.x;  // Position X
            mesh->vertices[i + 1] *= scale.y;  // Position Y
            mesh->vertices[i + 2] *= scale.z;  // Position Z
            // Colors stay the same (indices 3, 4, 5)
        }
        
        scaled[mesh_idx] = 1;
    }
}



