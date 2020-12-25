/**#version 410
void main(void){
    gl_FragColor = vec4(1.0,0.0,0.0,1.0);
}
**/
#version 410
out vec4 color;
layout (location = 0) in vec4 vertex; // recibe el vertice del Vertex Shader
void main(void){
    color = vertex;
}
