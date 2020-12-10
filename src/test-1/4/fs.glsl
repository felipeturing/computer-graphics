#version 410
out vec4 color;
layout (location = 0) in vec4 vertexColor; // recibe el vertice del Vertex Shader
void main(void){
    color = vertexColor;
}
