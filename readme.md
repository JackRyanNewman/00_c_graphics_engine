
## Engine overview and how to make scenes
This project is a lightweight 3D graphics engine using openGL 3.3, written to load pre-created scenes. The current scene is rendered as a room scene with multiple spinning objects all with shaders. The rotation is applied every frame, 60frames per second. The engine is organized around a clear separation of responsibilities: main initializes the system and hands control to the engine, the engine manages scene loading and runs a fixed 60 FPS render loop, and the renderer handles camera initialization, shader compilation, static mesh uploads to GPU buffers, and issuing draw calls. 

Scene management operates at a higher level on the CPU, updating per-object transforms by manipulating vector-based position, rotation, and scale data, while a dedicated math library converts these representations into optimized 4×4 model, view, and projection matrices. Static mesh assets are uploaded once and reused across the scene, allowing unique objects to be created by pairing shared geometry with different shaders and materials. Each frame, transformation updates and matrix calculations are parallelized on the CPU using OpenMP, while the GPU performs vertex transformation and rasterization through GLSL shaders, resulting in a clean, explicit, and data-oriented rendering pipeline. 

-> To edit the current scenes, go to 0_engine//2_scene. Looking in scece.c and c.h  
```c
typedef struct { 
    Mesh* parent_mesh;     // pointer to a mesh (const)
    Material* material;    // pointer to a material (const)
    Transform* transform;  // pointer to mutable transform
    GLenum draw_mode;      // wireframe vs full
} Rendered_Object;

Rendered_Object* scene_1(...) {
    static Transform walls = { ... };

    static Rendered_Object scene[] = {
        { .parent_mesh = &Room_Mesh,    .material = &Base_Color, .transform = &walls, .draw_mode = GL_TRIANGLES },
        { .parent_mesh = &Wire_Cube,    .material = &Base_Color, .transform = &obj_1, .draw_mode = GL_LINES },
        { .parent_mesh = &Full_Diamond, .material = &Base_Color, .transform = &obj_2, .draw_mode = GL_TRIANGLES },
        { .parent_mesh = &Wire_Diamond, .material = &Base_Color, .transform = &obj_3, .draw_mode = GL_LINES },
        { .parent_mesh = &Wire_Diamond, .material = &Base_Color, .transform = &obj_4, .draw_mode = GL_LINES }
    };

    return scene;
}
```
‎This method constructs rendered objects by referencing static assets defined in the 0_engine/2_scene/static_assets folder. To place an object in the scene, you must specify its transform, the mesh it uses, its materials, and its draw mode. Meshes and materials are reused from the static assets (with materials optionally combined at the scene level), while transforms are used to create “unique” rendered instances. To add new objects to the scene, simply add new meshes to the static assets mesh file and reference them accordingly, or mix and match with what I currenlty have! 

