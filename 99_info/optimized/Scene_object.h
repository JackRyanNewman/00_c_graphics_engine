  //--------------------------------------------------------------------
  //Organizational Hiercharchy for materials. 
  typedef struct Scene {
      const short max_objects;
      const Mesh* const* parent_meshes;             // 1D array of const Mesh pointers
      const Material* const* const* material_group; // 2D array: [mesh][material]
      Transform* const* const* transforms;          // 3D array: [mesh][material][instance], pointer const, transforms mutable
  } Scene;
    
  typedef struct Mesh_Group{
    const Mesh parent_mesh; 
    const Material *material_group; 
  } Mesh_Group;

  typedef struct Object_Group{
    const Material material; 
    Transform *transforms;
  } Object_Group;