#version 410
#define MPI 3.1415

uniform float numi;

vec4 sinx(float x) {
    return vec4(x, sin(x*2*MPI)/2.0, 0.0, 1.0);
}
void main(void) {

    //definiendo los puntos a evaluar para cada instance (osea para cada linea)
    // todas las lineas van a compartir un extremo para que no haya huecos y mientras más lineas mayor resolución
    // de la gráfica
    float x1 = 2 * gl_InstanceID / numi - 1;
    float x2 = 2 * (gl_InstanceID + 1) / numi - 1;


    //definiendo las posiciones evaluandos los puntos
    if (gl_VertexID == 0)
        gl_Position = sinx(x1);
    else
        gl_Position = sinx(x2);
}
