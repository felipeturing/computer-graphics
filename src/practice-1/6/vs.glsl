#version 410
layout (location = 0) out vec4 vertex; // Se envia el vertice al Fragment Shader
void main(void){
    if(gl_VertexID == 0) gl_Position = vec4(-0.25,0.0,0.0,1.0) ;
    else if(gl_VertexID == 1) gl_Position = vec4(0.25,0.0,0.0,1.0) ;
    else gl_Position = vec4(0.0,0.25,0.0,1.0);
    vertex = gl_Position;
}
