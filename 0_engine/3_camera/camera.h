/* Jack Newman*/

/*=================================================================================================*/
/*External Libraries*/
#ifndef CAM_H
#define CAM_H
#include "../5_math/math3d.h"




/*=================================================================================================*/
/* Defines */

#define DEFAULT_NEAR_PLANE 0.1f
#define DEFAULT_FAR_PLANE 100.0f
#define DEFAULT_CAMERA_DISTANCE 5.0f      // Closer to see the room
#define DEFAULT_FOV (M_PI / 3.0f)         // Keep 60 degrees

/*=================================================================================================*/
/* Structures  and Global Variables */
/* None - camera state is internal */

/*=================================================================================================*/
/* Forward Declarations */

  /*=========================================================================*/
  /* ========== Camera Lifecycle ========== */
  void camera_init(float t_aspect);

  /*=========================================================================*/
  /* ========== Camera Matrices ========== */
  Mat4 camera_get_view_matrix();
  Mat4 camera_get_projection_matrix();

  /*=========================================================================*/
  /* ========== Camera Control ========== */
  void camera_set_distance(float distance);
  float camera_get_distance();

/*=================================================================================================*/
/* MACROS */
#endif