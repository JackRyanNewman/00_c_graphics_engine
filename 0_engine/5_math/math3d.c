/*┌─────────────────────────────────────────────────────────────────────────────┐
	│ CNN - Graphics Math Library                                                 │
	│   • © 2025 Jack Ryan Newman                                                 │
	│   • Optimized code with detailed mathematical explanations                  │
	├─────────────────────────────────────────────────────────────────────────────┤
	└─────────────────────────────────────────────────────────────────────────────┘*/
/*=================================================================================================*/
#include "math3d.h"


/*=================================================================================================*/
// ========== Additional Notes ==========
	/* COLUMN-MAJOR STORAGE (OpenGL convention)
		Memory layout:  * Access formula: m[row + col×4]
			[ 0   4   8  12 ]
			[ 1   5   9  13 ]
			[ 2   6  10  14 ]
			[ 3   7  11  15 ]
 	*/

	//To do
	//otpimizee 
		//Controlled updates: certain things only update per x frame
		//Animation Frame controll: 
		//Afffine transformtions. 
		//Static Rotation Matrix. 
		//Shared transformations

/*=================================================================================================*/
/*Basic Matrix Informations*/


	Mat4 mat4_identity(Vec3 position){
		/* Translation Matrix from Identity
			1. Identity Matrix: The multiplicative identity (I×M=M×I=M). The I's controll the scale of the item. 
			2. Affine Transformation: Preserves parallel lines through translation, enables postion change. 
			3. Homogeneous Coordinates: The w-component determines behavior
				Points have w=1, so they ARE affected by translation
				Vectors have w=0, so they are NOT affected (directions don't move)
			Structure:
			[ i   0   0   tx ]     When multiplied with point [x,y,z,w]:
			[ 0   i   0   ty ]     x' = i*x + tx*w
			[ 0   0   i   tz ]     y' = i*y + ty*w
			[ 0   0   0   i  ]     z' = i*z + tz*w
															w' = i*w
		For points: w=1, translation IS applied (x' = x + tx, y' = y + ty, z' = z + tz)
		For vectors: w=0, translation NOT applied (x' = x, y' = y, z' = z)
		*/
		//Idienty and translation
		Mat4 m = {0};
		m.m[0]  = 1.0f; m.m[5]  = 1.0f;  
		m.m[10] = 1.0f; m.m[15] = 1.0f;  
		m.m[12] = position.x;  // tx
		m.m[13] = position.y;  // ty
		m.m[14] = position.z;  // tz
		return m;
	}

	Mat4 mat4_unique_identity(Vec3 scale, Vec3 position) {
			Mat4 m = {0}; 
			m.m[0] = scale.x;  m.m[5] = scale.y; 
			m.m[10] = scale.z; m.m[15] = 1.0f;  // Set diagonal to 1
			m.m[12] = position.x; 
			m.m[13] = position.y; 
			m.m[14] = position.z;
			return m;
	}

	/**
	 * Rotation Matrix from Axis-Angle (Rodrigues' Rotation Formula)
	 * ──────────────────────────────────────────────────────────────
	 * Rotate by angle θ around arbitrary unit axis (Rx, Ry, Rz):
	 * 
	 * [ Cθ + Rx²*OCθ         RxRy*OCθ - Rz*Sθ    RxRz*OCθ + Ry*Sθ   0 ]
	 * [ RxRy*OCθ + Rz*Sθ     Cθ + Ry²*OCθ        RyRz*OCθ - Rx*Sθ   0 ]
	 * [ RxRz*OCθ - Ry*Sθ     RyRz*OCθ + Rx*Sθ    Cθ + Rz²*OCθ       0 ]
	 * [ 0                    0                   0                  1 ]
	 * 
	 * Combined with Scale & Translation:
	 * [ i1  0   0   X ]      [ R00  R01  R02  0 ]
	 * [ 0   i2  0   Y ]  ×   [ R10  R11  R12  0 ]
	 * [ 0   0   i3  Z ]      [ R20  R21  R22  0 ]
	 * [ 0   0   0   1 ]      [ 0    0    0    1 ]
	 */
	Mat4 mat4_rotate(Vec3 scale, Vec3 position, Vec4 quaternion) {
			Mat4 m = {0};
			
			// Extract scale factors
			float i1 = scale.x, i2 = scale.y, i3 = scale.z;
			
			// Extract axis from quaternion.xyz (should be unit vector)
			float Rx = quaternion.x, Ry = quaternion.y, Rz = quaternion.z;
			
			// Extract angle from quaternion.w
			float angle = quaternion.w;
			
			// Precompute trig and reusable terms
			float Cθ = cosf(angle);                    // cos(θ)
			float Sθ = sinf(angle);                    // sin(θ)
			float OCθ = 1.0f - Cθ;                     // 1 - cos(θ)
			
			// Precompute squared axis components
			float Rx2 = Rx * Rx;
			float Ry2 = Ry * Ry;
			float Rz2 = Rz * Rz;
			
			// Precompute axis products
			float RxRy = Rx * Ry;
			float RxRz = Rx * Rz;
			float RyRz = Ry * Rz;
			
			// Row 0: Rotation + Scale
			m.m[0]  = i1 * (Cθ + Rx2 * OCθ);           // R00
			m.m[1]  = i1 * (RxRy * OCθ - Rz * Sθ);     // R01
			m.m[2]  = i1 * (RxRz * OCθ + Ry * Sθ);     // R02
			m.m[3]  = 0.0f;
			
			// Row 1: Rotation + Scale
			m.m[4]  = i2 * (RxRy * OCθ + Rz * Sθ);     // R10
			m.m[5]  = i2 * (Cθ + Ry2 * OCθ);           // R11
			m.m[6]  = i2 * (RyRz * OCθ - Rx * Sθ);     // R12
			m.m[7]  = 0.0f;
			
			// Row 2: Rotation + Scale
			m.m[8]  = i3 * (RxRz * OCθ - Ry * Sθ);     // R20
			m.m[9]  = i3 * (RyRz * OCθ + Rx * Sθ);     // R21
			m.m[10] = i3 * (Cθ + Rz2 * OCθ);           // R22
			m.m[11] = 0.0f;
			
			// Row 3: Translation + Homogeneous coordinate
			m.m[12] = position.x;                      // X (translation)
			m.m[13] = position.y;                      // Y (translation)
			m.m[14] = position.z;                      // Z (translation)
			m.m[15] = 1.0f;                            // w (homogeneous)
			
			return m;
	}



/*=================================================================================================*/
// ========== Camera Functinoality ==========

	/** Perspective Projection Matrix
	 * ──────────────────────────────────────────────────────────────
	 * Mathematical Principle: Models how the human eye sees depth
	 * 
	 * Key Concept: Objects farther away appear smaller (perspective foreshortening)
	 * 
	 * The Frustum:
							near plane          far plane
									|                  |
				\        |        /         |
			eye \       |       /          |
					\______|______/           |
						\     |     /            |
						\    |    /             |
							\   |   /              |
							\  |  /               |
								\ | /                |
								\|/________________/

		* This matrix performs two key operations:
		* 1. SCALING: Based on field of view (fov) and aspect ratio
			- Horizontal scale: 1/(aspect × tan(fov/2))
			- Vertical scale: 1/tan(fov/2)
			- Larger fov = smaller scale = more visible area
		* 
		* 2. DEPTH MAPPING: Maps [near, far] depth range to [-1, 1] NDC
			- Uses nonlinear mapping: more precision near camera
			- Formula: z' = (far+near)/(near-far) + (2×far×near)/(near-far)×(1/z)
		* 
		* 3. PERSPECTIVE DIVIDE: Sets w = -z, so later division by w creates perspective
			- After transform: (x, y, z, w) where w = -z_original
			- GPU divides: (x/w, y/w, z/w) creating the "shrinking" effect
		* 
		* Parameters:
		fov: Field of view angle (radians) - wider angle = more visible
		aspect: screen_width / screen_height
		near_plane: closest visible distance (z > 0)
		far_plane: farthest visible distance
	 */
	Mat4 mat4_perspective(float fov, float aspect, float near_plane, float far_plane) {
			Mat4 m = {0};
			float tan_half_fov = tanf(fov / 2.0f);
			
			
			m.m[0] = 1.0f / (aspect * tan_half_fov); // X-axis scale (adjusted for aspect ratio to prevent stretching)
			m.m[5] = 1.0f / tan_half_fov; 	         // Y-axis scale (controls vertical field of view)
			
			// Z-axis: nonlinear depth mapping to normalized device coordinates
			m.m[10] = -(far_plane + near_plane) / (far_plane - near_plane);
			m.m[14] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);
			
			// Enable perspective divide: copy -z into w component
			m.m[11] = -1.0f;
			
			return m;
	}

	/** Look-At View Matrix (Camera Transform)
	 * ──────────────────────────────────────────────────────────────
	 * Mathematical Principle: Change of basis from world space to camera space
	 * 
	 * Concept: Define a new coordinate system where:
	 - Origin is at the camera position (eye)
		- Z-axis points toward the camera (opposite of view direction)
		- Y-axis points "up" relative to the camera
		- X-axis points "right" relative to the camera
		* 
		* Process:
		* 1. Create orthonormal basis (3 perpendicular unit vectors):
			
			Forward (f) = normalize(center - eye)  [where camera looks]
			Right (s)   = normalize(f × up)        [camera's right direction]
			Up (u)      = s × f                    [camera's true up]
		* 
		* 2. Build rotation matrix using basis vectors as rows:
			[ sx  sy  sz  0 ]
			[ ux  uy  uz  0 ]
			[-fx -fy -fz  0 ]  (negated because OpenGL looks down -Z)
			[ 0   0   0   1 ]
		* 
		* 3. Add translation to move world relative to camera:
			Translation = -(rotation × eye_position)
			This effectively moves the world so camera is at origin
		* 
		* Why Cross Products?
		The cross product a × b produces a vector perpendicular to both a and b
		This ensures our coordinate axes are orthogonal (90° apart)
		* 
		* Parameters:
		eye: camera position in world space
		center: point the camera looks at
		up: which direction is "up" (usually (0,1,0))
	 */
	Mat4 mat4_look_at(Vec3 eye, Vec3 center, Vec3 up) {
			// Forward vector: direction from eye to target
			Vec3 f = {center.x - eye.x, center.y - eye.y, center.z - eye.z};
			float len = sqrtf(f.x*f.x + f.y*f.y + f.z*f.z);
			f.x /= len; f.y /= len; f.z /= len;  // Normalize
			
			// Right vector: perpendicular to forward and up (cross product)
			Vec3 s = {f.y*up.z - f.z*up.y, f.z*up.x - f.x*up.z, f.x*up.y - f.y*up.x};
			len = sqrtf(s.x*s.x + s.y*s.y + s.z*s.z);
			s.x /= len; s.y /= len; s.z /= len;  // Normalize
			
			// True up vector: perpendicular to both right and forward
			Vec3 u = {s.y*f.z - s.z*f.y, s.z*f.x - s.x*f.z, s.x*f.y - s.y*f.x};
			
			// Build the view matrix (rotation + translation combined)
			Mat4 m = {0};
			m.m[0] = s.x;  m.m[4] = s.y;  m.m[8]  = s.z;   // Right vector
			m.m[1] = u.x;  m.m[5] = u.y;  m.m[9]  = u.z;   // Up vector
			m.m[2] = -f.x; m.m[6] = -f.y; m.m[10] = -f.z;  // Forward (negated)
			
			// Translation: dot product projects eye position onto each axis
			m.m[12] = -(s.x*eye.x + s.y*eye.y + s.z*eye.z);
			m.m[13] = -(u.x*eye.x + u.y*eye.y + u.z*eye.z);
			m.m[14] = f.x*eye.x + f.y*eye.y + f.z*eye.z;
			m.m[15] = 1.0f;
			return m;
	}
/*=================================================================================================*/
/*Randomize Informations*/
		uint64_t s0 = 0x123456789abcdefULL, s1 = 0xfedcba987654321ULL;
		float xoroshiro128_next_range(float min, float max) {
				return min + (float)(xoroshiro128_next() % (uint64_t)(max - min + 1));
		}
		
		inline uint64_t xoroshiro128_next() {
				uint64_t result = s0 + s1;
				uint64_t t = s1 ^ s0;
				s0 = ((s0 << 55) | (s0 >> (64 - 55))) ^ t ^ (t << 14);
				s1 = (t << 36) | (t >> (64 - 36));
				return result;
		}

//======================================================================================================
//Thoughts
	//On Old translation
		//Translation should only occur if its a moveable object? Else it should be static information? 
		//Scale should only occur if its changable object? Else it should be static information? 