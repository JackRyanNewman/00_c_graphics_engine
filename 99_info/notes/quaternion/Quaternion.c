typedef struct { float x, y, z, w; } Quaternion;
typedef struct { float x, y, z;} Vec3;




/*=================================================================================================*/
/*Rotating*/


Quaternion quaternion_multiply(Quaternion a, Quaternion b) {
    // Combines two quaternions (rotate by two rotations in sequence)
    Quaternion result;
    
    result.w = a.w*b.w - a.x*b.x - a.y*b.y - a.z*b.z;
    result.x = a.w*b.x + a.x*b.w + a.y*b.z - a.z*b.y;
    result.y = a.w*b.y - a.x*b.z + a.y*b.w + a.z*b.x;
    result.z = a.w*b.z + a.x*b.y - a.y*b.x + a.z*b.w;
    
    return result;
}

Quaternion quaternion_from_axis_angle(Vec3 axis, float angle) {
  // Quaternion from Axis-Angle (single axis) 
   //Normalize the axis. 

    //Techinally we could remove this normilization, if we gaurteed our pre-comp totals to 1! 
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


Quaternion quaternion_from_euler(float angle_x, float angle_y, float angle_z) {
 // Quaternion from Euler Angles (all 3)
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

/*=================================================================================================*/






Quaternion quaternion_normalize(Quaternion q) {
    float len = sqrtf(q.x*q.x + q.y*q.y + q.z*q.z + q.w*q.w);
    q.x /= len;
    q.y /= len;
    q.z /= len;
    q.w /= len;
    return q;
}

