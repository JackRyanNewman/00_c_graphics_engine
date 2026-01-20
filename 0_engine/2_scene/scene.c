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
//static Vec3 base_rotation = {1,1,1}; 
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
    // Axis-Angle rotation: quaternion.xyz = axis, quaternion.w = angle (radians)
    
    // Define rotation axis (normalized unit vector)
    // This stays constant - we're rotating around a fixed axis
    obj->quaternion.x = 1.0f;  // X-axis
    obj->quaternion.y = 0.0f;  // 0 contribution
    obj->quaternion.z = 0.0f;  // 0 contribution

    float len = sqrtf(obj->quaternion.x*obj->quaternion.x + obj->quaternion.y*obj->quaternion.y + obj->quaternion.z*obj->quaternion.z);
    obj->quaternion.x /= len; obj->quaternion.y /= len; obj->quaternion.z /= len;
    
    obj->quaternion.w += 1.0f * dt;                                    //Increment angle, rotation speed //(wraps to prevent infinite growth)
    if (obj->quaternion.w > 2.0f*M_PI) obj->quaternion.w -= 2.0f*M_PI; // Wrap angle between 0 and 2π to prevent accumulation
    
}

Mat4 Transform_get_model_matrix(Transform* obj) {
    // Build transformation matrix from position, quaternion, and scale
    return mat4_rotate(obj->scale, obj->position, obj->quaternion);
}

void scene_cleanup() {}
//


/*=================================================================================================*/
/*Methods*/
Rendered_Object* scene_1(int* total_objs, Mesh*** mesh_lst, int* num_meshes, Material*** mat_lst, int* num_mats){
    // Static transforms (rotating objects)
    
     static Transform walls = { .actions=NULL, .position={0, 0, 0},  .scale={1, 1, 1}, .quaternion={0, 0, 0, 1} };
     static Transform obj_1 = { .actions=NULL, .position={2, 0, 0},  .scale={1, 1, 1}, .quaternion={0, 0, 0, 1} };
     static Transform obj_2 = { .actions=NULL, .position={-2, 0, 0}, .scale={1, 1, 1}, .quaternion={0, 0, 0, 1} };
     static Transform obj_3 = { .actions=NULL, .position={0, 2, 0},  .scale={1, 1, 1}, .quaternion={0, 0, 0, 1} };

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
    static Transform* rotating[] = { &obj_1, &obj_2, &obj_3 };
    total_rotating = sizeof(rotating)/sizeof(rotating[0]);


    Rotating_Objects = rotating;

    return scene;

}



