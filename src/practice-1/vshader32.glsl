#version 410
#define M_PI 3.14159265358

mat4 buildRotateX(float grad) {
    float rad = grad * M_PI/180.0;
    mat4 xrot = mat4(1.0, 0.0,      0.0,       0.0,
                    0.0, cos(rad), -sin(rad), 0.0,
                    0.0, sin(rad), cos(rad),  0.0,
                    0.0, 0.0,      0.0,       1.0);
    return xrot;
}
mat4 buildRotateY(float grad) {
    float rad = grad * M_PI/180.0;
    mat4 yrot = mat4(cos(rad),  0.0, sin(rad), 0.0,
                   0.0,       1.0, 0.0,      0.0,
                   -sin(rad), 0.0, cos(rad), 0.0,
                   0.0,       0.0, 0.0,      1.0);
    return yrot;
}

// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float grad) {
    float rad = grad * M_PI/180.0;
    mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
                   sin(rad), cos(rad),  0.0, 0.0,
                   0.0,      0.0,       1.0, 0.0,
                   0.0,      0.0,       0.0, 1.0);
    return zrot;
}

void main(void) {
    mat4 rotatex = buildRotateX(0);
    mat4 rotatez = buildRotateZ(0);
    mat4 rotatey = buildRotateY(0);
    vec4 myvec;
    if (gl_VertexID == 0) myvec = vec4(0.25, -0.25, 0.0, 1.0);
    else if (gl_VertexID == 1) myvec = vec4(-0.25, -0.25, 0.0, 1.0);
    else myvec = vec4(0., 0., 0.0, 1.0);
    gl_Position = rotatey * myvec;
}
