#version 410
#define M_PI 3.14159265358
void main(void){

    float angle = -2.0 + gl_VertexID*(4.0/64.0);
    float sinangle = sin(angle*M_PI);

    /*
    * La caja [-2pi, 2pi]*[-1, 1] lo cambio a la caja [-1,1]*[1/2,1/2]
    * para que sea visible tal cual la imagen dada en la practica
    */
    float normalizeangle =  angle/2.0;
    float normalizesinangle = sinangle/2.0;

    gl_Position = vec4(normalizeangle,normalizesinangle,0.0,1.0);

}
