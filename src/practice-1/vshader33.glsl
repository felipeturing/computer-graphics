#version 410
layout (location = 0) out vec4 vertexColor;
void main(void){
    if (gl_VertexID == 0) gl_Position = vec4(0.75, 0.0, 0.0,1.0);
    else if (gl_VertexID == 1) gl_Position = vec4(0.0, 0.0, 0.5, 1.0);
    else gl_Position = vec4(0.0, 0.75, 0.0, 1.0);
    vertexColor = gl_Position;
}
