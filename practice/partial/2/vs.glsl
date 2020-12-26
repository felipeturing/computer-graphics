#version 430

layout (location = 0) in vec3 position;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform float tf;
uniform int random;
mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildTranslate(float x, float y, float z);

void main(void)
{
        mat4 newM_matrix, localRotX, localRotY,localRotZ, localTrans;
        float a,b,c;
//    float i = gl_InstanceID + tf + random ;
        float i = gl_InstanceID + tf + random;

        a = sin(0.15 * i) * 15.0;
        b = sin(0.52 * i) * 15.0;
        c = sin(0.90 * i) * 15.0;
        localTrans = buildTranslate(a, b,c);
        localRotX = buildRotateX(0.2 * i);
        localRotY = buildRotateY(1.0 * i);
        localRotZ = buildRotateZ(1.75 * i);
        newM_matrix = localTrans * localRotX * localRotY * localRotZ;


        //al descomentar esto y comentar desde la linea 21 a la 28, da ciertas animaciones interesantes
        /*if(random == 0){
            a = sin(0.15 * i) * 15.0;
            localTrans = buildTranslate(a, 0.0, 0.0);
            localRotX = buildRotateX(0.2 * i);
            newM_matrix = localTrans * localRotX;
        }else if(random == 1){
             b = sin(0.52 * i) * 15.0;
            localRotY = buildRotateY(1.0 * i);
             localTrans = buildTranslate(0.0, b, 0.0);
            newM_matrix = localTrans * localRotY ;
        }else if(random == 2){
            c = sin(0.90 * i) * 15.0;
            localRotZ = buildRotateZ(1.75 * i);
             localTrans = buildTranslate(0.0, 0.0, c);
             newM_matrix = localTrans * localRotZ ;
        }else if(random == 3){
            a = sin(0.15 * i) * 15.0;
            localRotY = buildRotateY(1.0 * i);
            localTrans = buildTranslate(a, 0.0, 0.0);
             newM_matrix = localTrans * localRotY ;
        }else if(random == 4){
            a = sin(0.15 * i) * 15.0;
            localRotZ = buildRotateZ(1.75 * i);
             localTrans = buildTranslate(a, 0.0, 0.0);
             newM_matrix = localTrans * localRotZ ;
        }else if(random == 5){
           b = sin(0.52 * i) * 15.0;
            localRotX = buildRotateX(0.2 * i);
            localTrans = buildTranslate(0.0, b, 0.0);
           newM_matrix = localTrans * localRotX ;
        }else if(random == 6){
           b = sin(0.52 * i) * 15.0;
            localRotZ = buildRotateZ(1.75 * i);
          localTrans = buildTranslate(0.0, b, 0.0);
          newM_matrix = localTrans * localRotZ ;
        }else if(random == 7){
           c = sin(0.90 * i) * 15.0;
          localRotX = buildRotateX(0.2 * i);
            localTrans = buildTranslate(0.0, 0.0, c);
             newM_matrix = localTrans * localRotX ;
        }else if(random == 8){
            c = sin(0.90 * i) * 15.0;
           localRotY = buildRotateY(1.0 * i);
            localTrans = buildTranslate(0.0, 0.0, c);
           newM_matrix = localTrans * localRotY ;
        }else{
             c = sin(0.90 * i) * 15.0;
            localRotY = buildRotateY(1.0 * i);
             localTrans = buildTranslate(0.0, 0.0, c);
           newM_matrix = localTrans * localRotY ;
        }*/

    mat4 new_mv_matrix = mv_matrix * newM_matrix;
    gl_Position = proj_matrix * new_mv_matrix * vec4(position,1.0);
}


// builds and returns a matrix that performs a rotation around the X axis
mat4 buildRotateX(float rad) {
    mat4 xrot = mat4(1.0, 0.0,      0.0,       0.0,
                     0.0, cos(rad), -sin(rad), 0.0,
                     0.0, sin(rad), cos(rad),  0.0,
                     0.0, 0.0,      0.0,       1.0);
    return xrot;
}

// builds and returns a matrix that performs a rotation around the Y axis
mat4 buildRotateY(float rad) {
    mat4 yrot = mat4(cos(rad),  0.0, sin(rad), 0.0,
                     0.0,       1.0, 0.0,      0.0,
                     -sin(rad), 0.0, cos(rad), 0.0,
                     0.0,       0.0, 0.0,      1.0);
    return yrot;
}

// builds and returns a matrix that performs a rotation around the Z axis
mat4 buildRotateZ(float rad) {
    mat4 zrot = mat4(cos(rad), -sin(rad), 0.0, 0.0,
                     sin(rad), cos(rad),  0.0, 0.0,
                     0.0,      0.0,       1.0, 0.0,
                     0.0,      0.0,       0.0, 1.0);
    return zrot;
}

// builds and returns a translation matrix
mat4 buildTranslate(float x, float y, float z) {
    mat4 trans = mat4(1.0, 0.0, 0.0, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      x,   y,   z,   1.0);
    return trans;
}
