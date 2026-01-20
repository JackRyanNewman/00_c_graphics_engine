/* Jack Newman*/

/*=================================================================================================*/
/*External Libraries*/
#ifndef MATH3D_H
#define MATH3D_H
#include <math.h>

/*=================================================================================================*/
/* Defines */
#define M_PI 3.14159265358979323846


/*=================================================================================================*/
/* Structures */

typedef struct Vec3{
    float x, y, z;
} Vec3;


typedef struct Mat4 {
    float m[16];  // Column-major 4x4 matrix
} Mat4;

/*=================================================================================================*/
/* Global Variables */
/* None */

/*=================================================================================================*/
/* Forward Declarations */

  /*=========================================================================*/
  /* ========== Matrix Creation ========== */
  Mat4 mat4_identity();
  Mat4 mat4_perspective(float fov, float aspect, float near_plane, float far_plane);
  Mat4 mat4_look_at(Vec3 eye, Vec3 center, Vec3 up);

  /*=========================================================================*/
  /* ========== Transformations ========== */
  Mat4 mat4_translate(Vec3 translation);
  Mat4 mat4_rotate_x(float angle);
  Mat4 mat4_rotate_y(float angle);
  Mat4 mat4_rotate_z(float angle);

  /*=========================================================================*/
  /* ========== Matrix Operations ========== */
  Mat4 mat4_multiply(Mat4 a, Mat4 b);

/*=================================================================================================*/
/* MACROS */
#endif