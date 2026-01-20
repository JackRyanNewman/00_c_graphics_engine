typedef struct {
    float x, y, z, w;
} Quaternion;

typedef struct {
    float x, y, z;
} Vec3;

// ============================================
// Normalize a Quaternion
// ============================================
Quaternion quaternion_normalize(Quaternion q) {
    float len = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    q.x /= len;
    q.y /= len;
    q.z /= len;
    q.w /= len;
    return q;
}

// ============================================
// Quaternion from Axis-Angle (single axis)
// ============================================
Quaternion quaternion_from_axis_angle(Vec3 axis, float angle) {
    // Normalize the axis
    float len = sqrtf(axis.x*axis.x + axis.y*axis.y + axis.z*axis.z);
    axis.x /= len;
    axis.y /= len;
    axis.z /= len;
    
    float half_angle = angle * 0.5f;
    float s = sinf(half_angle);
    
    Quaternion q;
    q.x = axis.x * s;
    q.y = axis.y * s;
    q.z = axis.z * s;
    q.w = cosf(half_angle);
    
    return q;
}

// ============================================
// Quaternion from Euler Angles (all 3)
// ============================================
Quaternion quaternion_from_euler(float angle_x, float angle_y, float angle_z) {
    float hx = angle_x * 0.5f;
    float hy = angle_y * 0.5f;
    float hz = angle_z * 0.5f;
    
    float sin_hx = sinf(hx), cos_hx = cosf(hx);
    float sin_hy = sinf(hy), cos_hy = cosf(hy);
    float sin_hz = sinf(hz), cos_hz = cosf(hz);
    
    Quaternion q;
    q.w = cos_hx * cos_hy * cos_hz + sin_hx * sin_hy * sin_hz;
    q.x = sin_hx * cos_hy * cos_hz - cos_hx * sin_hy * sin_hz;
    q.y = cos_hx * sin_hy * cos_hz + sin_hx * cos_hy * sin_hz;
    q.z = cos_hx * cos_hy * sin_hz - sin_hx * sin_hy * cos_hz;
    
    return q;
}

// ============================================
// Quaternion Multiplication
// Combines two quaternions (rotate by two rotations in sequence)
// ============================================
Quaternion quaternion_multiply(Quaternion a, Quaternion b) {
    Quaternion result;
    
    result.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
    result.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
    result.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
    result.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
    
    return result;
}

// ============================================
// Update a Quaternion by Rotations
// Pass Vec3 with angles and single angle parameter
// ============================================
Quaternion quaternion_rotate(Quaternion q, Vec3 rotation_angles, float angle) {
    // Create delta rotation from Vec3 angles and single angle
    Quaternion delta = quaternion_from_euler(
        rotation_angles.x * angle,
        rotation_angles.y * angle,
        rotation_angles.z * angle
    );
    
    // Apply it by multiplying
    return quaternion_multiply(q, delta);
}

// ============================================
// Usage Examples
// ============================================

/*
// Example 1: Create from axis-angle
Quaternion q = quaternion_from_axis_angle((Vec3){1, 0, 0}, 0.087f);

// Example 2: Create from Euler angles
Quaternion q = quaternion_from_euler(0.1f, 0.2f, 0.3f);

// Example 3: Update rotation with Vec3 and angle scalar
Quaternion rot = quaternion_from_euler(0, 0, 0);
rot = quaternion_rotate(rot, (Vec3){1, 0, 0}, 0.087f);  // Rotate around X

// Example 4: Multiply two quaternions
Quaternion q1 = quaternion_from_euler(0.1f, 0, 0);
Quaternion q2 = quaternion_from_euler(0, 0.2f, 0);
Quaternion combined = quaternion_multiply(q1, q2);

// Example 5: Normalize if needed
Quaternion normalized = quaternion_normalize(combined);
*/