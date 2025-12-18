/* Jack Newman*/

/*=================================================================================================*/
/*External Libraries*/
#include <stdbool.h>
#include <GL/glew.h>      // GLEW MUST come first
#include <GLFW/glfw3.h>
#include "../2_scene/scene.h"


/*=================================================================================================*/
/* Defines */
/* None */

/*=================================================================================================*/
/* Structures & Global Variables*/
/* None - renderer is stateless from external perspective, None - all state is internal */

/*=================================================================================================*/
/* Forward Declarations */
  bool renderer_load_scene(int scene_index);
  bool geometry_create_mesh(Mesh* mesh);
  bool shader_create_program(Material *material);
  GLuint shader_compile(GLenum type, const char* source);
  void renderer_cleanup();
  void renderer_render_frame();

  // Rendered_Object *scene_functions(
  //   Transform*** out_rotating, int* out_rotating_len,
  //   Mesh*** out_meshes, int* out_meshes_len,
  //   Material*** out_materials, int* out_materials_len,
  //   int* total_objs
  // );
  
/*=================================================================================================*/
/* MACROS */
//scene_functions[scene_index]
// Rendered_Object* (*scene_functions[])(
//     Transform*** out_rotating, int* out_rotating_len,
//     Mesh*** out_meshes, int* out_meshes_len,
//     Material*** out_materials, int* out_materials_len,
//     int* total_objs
// ) = { scene_1 };