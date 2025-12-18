/* Jack Newman*/

/*=================================================================================================*/
/*External Libraries*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <omp.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/*=================================================================================================*/
/* Defines */
#define ENGINE_VERSION "1.0.0"
#define OP_Major_Ver 3
#define OP_minor_Ver 3

/*=================================================================================================*/
/* Structures */
/* None - engine is stateless from external perspective */

/*=================================================================================================*/
/* Global Variables */
/* None - all state is internal */

/*=================================================================================================*/
/* Forward Declarations */

  /*=========================================================================*/
  /* ========== Engine Lifecycle ========== */
  bool engine_start_up();
  bool engine_init_backend();
  void engine_run();
  void engine_clean();

  /*=========================================================================*/
  /* ========== Window Management ========== */
  GLFWwindow* engine_get_window();

/*=================================================================================================*/
/* MACROS */