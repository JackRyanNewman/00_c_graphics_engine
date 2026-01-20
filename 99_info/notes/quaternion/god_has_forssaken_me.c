void Transform_update(Transform* obj, float dt) {
    // Update rotation based on rotation speed
    obj->rotation.x += base_rotation.x * dt; 
    obj->rotation.y += base_rotation.y * dt;
    obj->rotation.z += base_rotation.z * dt;
}

Mat4 Transform_get_model_matrix(Transform* obj) {
    // Only translastion and rotation - scale already baked into vertices
    Mat4 translation = mat4_translate(obj->position);
    Mat4 rot_x = mat4_rotate_x(obj->rotation.x);
    Mat4 rot_y = mat4_rotate_y(obj->rotation.y);
    Mat4 rot_z = mat4_rotate_z(obj->rotation.z);
    Mat4 rotation = mat4_multiply(rot_z, mat4_multiply(rot_y, rot_x));
    
    return mat4_multiply(translation, rotation);
}