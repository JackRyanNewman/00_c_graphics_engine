/*┌─────────────────────────────────────────────────────────────────────────────┐
	│ CNN - Camera                                                                │
	│   • © 2025 Jack Ryan Newman                                                 │
	├─────────────────────────────────────────────────────────────────────────────┤
	└─────────────────────────────────────────────────────────────────────────────┘*/

/*=================================================================================================*/
/* Includes */
#include "camera.h"


/*=================================================================================================*/
/* Internal State */
static float camera_distance;
static float near_plane;
static float far_plane;
static float fov;
static float aspect;

static Vec3 eye;
static Vec3 center;
static Vec3 up;

/*=================================================================================================*/
/* Camera Lifecycle */

	void camera_init(float t_aspect) {
    camera_distance = DEFAULT_CAMERA_DISTANCE;
    near_plane = DEFAULT_NEAR_PLANE;
    far_plane = DEFAULT_FAR_PLANE;
    fov = DEFAULT_FOV;
    aspect = t_aspect;

    eye    = (Vec3){ 0.0f, 0.0f, camera_distance };
    center = (Vec3){ 0.0f, 0.0f, 0.0f };
    up     = (Vec3){ 0.0f, 1.0f, 0.0f };
}

/*=================================================================================================*/
/* Camera Matrices */
	float camera_get_distance() {return camera_distance;}	
	void camera_set_distance(float distance) { camera_distance = distance;}
	Mat4 camera_get_view_matrix() {return mat4_look_at(eye, center, up);}
	Mat4 camera_get_projection_matrix() { return mat4_perspective(fov, aspect, near_plane, far_plane);}
	
	void camera_set_eye(Vec3 new_eye) { eye = new_eye; } // Update camera position
	Vec3 camera_get_eye() { return eye;}  // Return current camera position


/*=================================================================================================*/
/* Camera Control */

	