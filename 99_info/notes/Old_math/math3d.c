/*┌─────────────────────────────────────────────────────────────────────────────┐
  │ CNN - Graphics Math Library                                                 │
  │   • © 2025 Jack Ryan Newman                                                 │
  │   • Optimized code with detailed mathematical explanations                  │
  ├─────────────────────────────────────────────────────────────────────────────┤
  └─────────────────────────────────────────────────────────────────────────────┘*/
/*=================================================================================================*/
#include "math3d.h"


//To do
//otpimizee 
  //Controlled updates: certain things only update per x frame
  //Animation Frame controll: 
  //Afffine transformtions. 
  //Static Rotation Matrix. 
  //Shared transformations















/*=================================================================================================*/
// ========== Matrix Creation Functions ==========

/**
 * Identity Matrix
 * ──────────────────────────────────────────────────────────────
 * Mathematical Principle: The multiplicative identity for matrices
 *   I × M = M × I = M  (identity property)
 * 
 * Structure (4×4):
 *   [ 1  0  0  0 ]     This matrix leaves any vector unchanged
 *   [ 0  1  0  0 ]     when multiplied. It's the "do nothing"
 *   [ 0  0  1  0 ]     transformation - analogous to multiplying
 *   [ 0  0  0  1 ]     by 1 in scalar arithmetic.
 * 
 * Memory Layout (column-major):
 *   [ 0   4   8  12 ]
 *   [ 1   5   9  13 ]
 *   [ 2   6  10  14 ]
 *   [ 3   7  11  15 ]
 */
Mat4 mat4_identity() {
    Mat4 m = {0};
    m.m[0] = m.m[5] = m.m[10] = m.m[15] = 1.0f;  // Set diagonal to 1
    return m;
}

/**
 * Perspective Projection Matrix
 * ──────────────────────────────────────────────────────────────
 * Mathematical Principle: Models how the human eye sees depth
 * 
 * Key Concept: Objects farther away appear smaller (perspective foreshortening)
 * 
 * The Frustum:
 *              near plane          far plane
 *                  |                  |
 *         \        |        /         |
 *      eye \       |       /          |
 *           \______|______/           |
 *            \     |     /            |
 *             \    |    /             |
 *              \   |   /              |
 *               \  |  /               |
 *                \ | /                |
 *                 \|/________________/
 * 
 * This matrix performs two key operations:
 * 1. SCALING: Based on field of view (fov) and aspect ratio
 *    - Horizontal scale: 1/(aspect × tan(fov/2))
 *    - Vertical scale: 1/tan(fov/2)
 *    - Larger fov = smaller scale = more visible area
 * 
 * 2. DEPTH MAPPING: Maps [near, far] depth range to [-1, 1] NDC
 *    - Uses nonlinear mapping: more precision near camera
 *    - Formula: z' = (far+near)/(near-far) + (2×far×near)/(near-far)×(1/z)
 * 
 * 3. PERSPECTIVE DIVIDE: Sets w = -z, so later division by w creates perspective
 *    - After transform: (x, y, z, w) where w = -z_original
 *    - GPU divides: (x/w, y/w, z/w) creating the "shrinking" effect
 * 
 * Parameters:
 *   fov: Field of view angle (radians) - wider angle = more visible
 *   aspect: screen_width / screen_height
 *   near_plane: closest visible distance (z > 0)
 *   far_plane: farthest visible distance
 */
Mat4 mat4_perspective(float fov, float aspect, float near_plane, float far_plane) {
    Mat4 m = {0};
    float tan_half_fov = tanf(fov / 2.0f);
    
    // X-axis scale (adjusted for aspect ratio to prevent stretching)
    m.m[0] = 1.0f / (aspect * tan_half_fov);
    
    // Y-axis scale (controls vertical field of view)
    m.m[5] = 1.0f / tan_half_fov;
    
    // Z-axis: nonlinear depth mapping to normalized device coordinates
    m.m[10] = -(far_plane + near_plane) / (far_plane - near_plane);
    m.m[14] = -(2.0f * far_plane * near_plane) / (far_plane - near_plane);
    
    // Enable perspective divide: copy -z into w component
    m.m[11] = -1.0f;
    
    return m;
}

/**
 * Look-At View Matrix (Camera Transform)
 * ──────────────────────────────────────────────────────────────
 * Mathematical Principle: Change of basis from world space to camera space
 * 
 * Concept: Define a new coordinate system where:
 *   - Origin is at the camera position (eye)
 *   - Z-axis points toward the camera (opposite of view direction)
 *   - Y-axis points "up" relative to the camera
 *   - X-axis points "right" relative to the camera
 * 
 * Process:
 * 1. Create orthonormal basis (3 perpendicular unit vectors):
 *    
 *    Forward (f) = normalize(center - eye)  [where camera looks]
 *    Right (s)   = normalize(f × up)        [camera's right direction]
 *    Up (u)      = s × f                    [camera's true up]
 * 
 * 2. Build rotation matrix using basis vectors as rows:
 *    [ sx  sy  sz  0 ]
 *    [ ux  uy  uz  0 ]
 *    [-fx -fy -fz  0 ]  (negated because OpenGL looks down -Z)
 *    [ 0   0   0   1 ]
 * 
 * 3. Add translation to move world relative to camera:
 *    Translation = -(rotation × eye_position)
 *    This effectively moves the world so camera is at origin
 * 
 * Why Cross Products?
 *   The cross product a × b produces a vector perpendicular to both a and b
 *   This ensures our coordinate axes are orthogonal (90° apart)
 * 
 * Parameters:
 *   eye: camera position in world space
 *   center: point the camera looks at
 *   up: which direction is "up" (usually (0,1,0))
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
// ========== Transformation Matrices ==========

/**
 * Translation Matrix
 * ──────────────────────────────────────────────────────────────
 * Mathematical Principle: Affine transformation (preserves parallel lines)
 * 
 * Structure:
 *   [ 1  0  0  tx ]     When multiplied with point (x,y,z,1):
 *   [ 0  1  0  ty ]     x' = x + tx
 *   [ 0  0  1  tz ]     y' = y + ty
 *   [ 0  0  0  1  ]     z' = z + tz
 * 
 * Why the 1 in homogeneous coordinates?
 *   Points have w=1, so they ARE affected by translation
 *   Vectors have w=0, so they are NOT affected (directions shouldn't move)
 */
Mat4 mat4_translate(Vec3 translation) {
    Mat4 m = mat4_identity();
    m.m[12] = translation.x;
    m.m[13] = translation.y;
    m.m[14] = translation.z;
    return m;
}

/**
 * Rotation Matrix - X Axis
 * ──────────────────────────────────────────────────────────────
 * Mathematical Principle: Rotation in the YZ plane (X unchanged)
 * 
 * Rodrigues' Rotation Formula specialized for X-axis:
 *   [ 1     0        0    ]
 *   [ 0  cos(θ)  -sin(θ) ]  Rotates counterclockwise when looking
 *   [ 0  sin(θ)   cos(θ) ]  down the positive X-axis toward origin
 * 
 * Geometric Interpretation:
 *   - X component stays the same (rotating around X-axis)
 *   - Y and Z components mix using sine and cosine
 *   - Positive angle rotates from +Y toward +Z
 * 
 * Why sine and cosine?
 *   They describe circular motion. Point at angle θ on unit circle:
 *   x = cos(θ), y = sin(θ)
 */
Mat4 mat4_rotate_x(float angle) {
    Mat4 m = mat4_identity();
    float c = cosf(angle), s = sinf(angle);
    m.m[5] = c;  m.m[6] = s;   // Y' = Y×cos - Z×sin (see row 1)
    m.m[9] = -s; m.m[10] = c;  // Z' = Y×sin + Z×cos (see row 2)
    return m;
}

/**
 * Rotation Matrix - Y Axis
 * ──────────────────────────────────────────────────────────────
 * Mathematical Principle: Rotation in the XZ plane (Y unchanged)
 * 
 * Structure:
 *   [ cos(θ)   0  sin(θ) ]
 *   [   0      1    0    ]  Rotates counterclockwise when looking
 *   [-sin(θ)   0  cos(θ) ]  down the positive Y-axis toward origin
 * 
 * Note the sign difference from X rotation:
 *   Positive angle rotates from +Z toward +X (right-hand rule)
 */
Mat4 mat4_rotate_y(float angle) {
    Mat4 m = mat4_identity();
    float c = cosf(angle), s = sinf(angle);
    m.m[0] = c;  m.m[2] = -s;  // X' =  X×cos + Z×sin
    m.m[8] = s;  m.m[10] = c;  // Z' = -X×sin + Z×cos
    return m;
}

/**
 * Rotation Matrix - Z Axis
 * ──────────────────────────────────────────────────────────────
 * Mathematical Principle: Rotation in the XY plane (Z unchanged)
 * 
 * Structure:
 *   [ cos(θ) -sin(θ)  0 ]
 *   [ sin(θ)  cos(θ)  0 ]  Rotates counterclockwise when looking
 *   [   0       0     1 ]  down the positive Z-axis toward origin
 * 
 * This is the "standard" 2D rotation extended to 3D
 *   Positive angle rotates from +X toward +Y
 */
Mat4 mat4_rotate_z(float angle) {
    Mat4 m = mat4_identity();
    float c = cosf(angle), s = sinf(angle);
    m.m[0] = c;  m.m[1] = s;   // X' = X×cos - Y×sin
    m.m[4] = -s; m.m[5] = c;   // Y' = X×sin + Y×cos
    return m;
}

/**
 * Matrix Multiplication
 * ──────────────────────────────────────────────────────────────
 * Mathematical Principle: Composition of linear transformations
 * 
 * WARNING: Matrix multiplication is NOT commutative!
 *   A × B ≠ B × A  (order matters!)
 * 
 * Interpretation of C = A × B:
 *   "Apply transformation B first, then apply transformation A"
 *   Read right-to-left: A(B(point))
 * 
 * Example:
 *   Rotate × Translate = "Move, then rotate around origin"
 *   Translate × Rotate = "Rotate around origin, then move"
 *   These produce DIFFERENT results!
 * 
 * Algorithm: Standard matrix multiplication
 *   C[i,j] = Σ(A[i,k] × B[k,j]) for k=0 to 3
 *   
 *   Each element is the dot product of:
 *     - Row i from matrix A
 *     - Column j from matrix B
 * 
 * Complexity: O(n³) for n×n matrices (here n=4, so 64 multiplies + 48 adds)
 */
Mat4 mat4_multiply(Mat4 a, Mat4 b) {
    Mat4 result = {0};
    for (int i = 0; i < 4; i++) {          // For each row of A
        for (int j = 0; j < 4; j++) {      // For each column of B
            for (int k = 0; k < 4; k++) {  // Dot product accumulation
                result.m[i + j*4] += a.m[i + k*4] * b.m[k + j*4];
            }
        }
    }
    return result;
}

/*=================================================================================================*/
// ========== Additional Notes ==========
/*
 * COLUMN-MAJOR STORAGE (OpenGL convention)
 * ───────────────────────────────────────────
 * Matrices are stored as 16 floats in column-major order:
 * 
 * Mathematical matrix:        Memory layout:
 *   [ m00 m01 m02 m03 ]        [ 0   4   8  12 ]
 *   [ m10 m11 m12 m13 ]        [ 1   5   9  13 ]
 *   [ m20 m21 m22 m23 ]        [ 2   6  10  14 ]
 *   [ m30 m31 m32 m33 ]        [ 3   7  11  15 ]
 * 
 * Access formula: m[row + col×4]
 * 
 * TRANSFORMATION ORDER
 * ───────────────────────────────────────────
 * Typical rendering pipeline:
 *   1. Model matrix: object's local transform (rotate, scale, position)
 *   2. View matrix: camera transform (look_at)
 *   3. Projection matrix: perspective
 * 
 * Combined: Projection × View × Model × Vertex
 * 
 * Remember: Matrices apply RIGHT to LEFT in multiplication!
 */