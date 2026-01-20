/*=================================================================================================*/
/* ========== Transformation Update ==========
		Loc/TranslationMatrix   Rotation Matrix
		[ i1  0   0   X ]      [ R00  R01  R02  0 ]
		[ 0   i2  0   Y ]  ×   [ R10  R11  R12  0 ]
		[ 0   0   i3  Z ]      [ R20  R21  R22  0 ]
		[ 0   0   0   i4]      [ 0    0    0    1 ]

		Result =
		[ i1*(1 − 2(y² + z²))      i1*2(xy − wz)           i1*2(xz + wy)          X ]
		[ i2*2(xy + wz)            i2*(1 − 2(x² + z²))     i2*2(yz − wx)          Y ]
		[ i3*2(xz − wy)            i3*2(yz + wx)           i3*(1 − 2(x² + y²))    Z ]
		[ 0                        0                       0                      1 ]
*/

//  Mat4 mat4_rotate(Vec3 scale, Vec3 position, Vec4 quaternion) {
//     Mat4 m = {0};
    
//     // Extract scale factors
//     float i1 = scale.x, i2 = scale.y, i3 = scale.z;
    
//     // Extract quaternion components
//     float x = quaternion.x, y = quaternion.y, z = quaternion.z, w = quaternion.w;
    
//     // Precompute squared and product terms (reused multiple times)
//     float x2 = x * x;
//     float y2 = y * y;
//     float z2 = z * z;
//     float xy = x * y;
//     float xz = x * z;
//     float yz = y * z;
//     float wx = w * x;
//     float wy = w * y;
//     float wz = w * z;
    
//     // Row 0: Rotation + Scale
//     m.m[0]  = i1 * (1.0f - 2.0f * (y2 + z2));      // R00
//     m.m[1]  = i1 * 2.0f * (xy - wz);               // R01
//     m.m[2]  = i1 * 2.0f * (xz + wy);               // R02
//     m.m[3]  = 0.0f;
    
//     // Row 1: Rotation + Scale
//     m.m[4]  = i2 * 2.0f * (xy + wz);               // R10
//     m.m[5]  = i2 * (1.0f - 2.0f * (x2 + z2));      // R11
//     m.m[6]  = i2 * 2.0f * (yz - wx);               // R12
//     m.m[7]  = 0.0f;
    
//     // Row 2: Rotation + Scale
//     m.m[8]  = i3 * 2.0f * (xz - wy);               // R20
//     m.m[9]  = i3 * 2.0f * (yz + wx);               // R21
//     m.m[10] = i3 * (1.0f - 2.0f * (x2 + y2));      // R22
//     m.m[11] = 0.0f;
    
//     // Row 3: Translation + Homogeneous coordinate
//     m.m[12] = position.x;                          // X (translation)
//     m.m[13] = position.y;                          // Y (translation)
//     m.m[14] = position.z;                          // Z (translation)
//     m.m[15] = 1.0f;                                // w (homogeneous)
//     return m;
// }


// void Transform_update(Transform* obj, float dt) {
//     // Update rotation based on rotation speed
//     obj->quaternion.x +=  1 * dt;  // Increment quaternion components
//     obj->quaternion.y +=  .5 * dt;
//     obj->quaternion.z +=  .5 * dt;
//     //obj->quaternion.w +=  xoroshiro128_next_range(-.3, .3) * dt;
//     // obj->quaternion.w stays unchanged (or increment if needed)
    
//     // Normalize quaternion to maintain unit length
//     float len = sqrtf(
//         obj->quaternion.x * obj->quaternion.x +
//         obj->quaternion.y * obj->quaternion.y +
//         obj->quaternion.z * obj->quaternion.z +
//         obj->quaternion.w * obj->quaternion.w
//     );
    
//     obj->quaternion.x /= len;  // Divide by length to normalize
//     obj->quaternion.y /= len;
//     obj->quaternion.z /= len;
//     obj->quaternion.w /= len;
// }