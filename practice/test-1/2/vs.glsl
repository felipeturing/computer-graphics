#version 410
layout (location = 0) out vec4 vertexColor; // Se envia el vertice al Fragment Shader
void main(void){

    if(gl_VertexID == 0) { gl_Position = vec4(-0.2,0.0,0.0,1.0); vertexColor=vec4(0.0,0.0,1.0,1.0);}
    else if(gl_VertexID == 1) { gl_Position = vec4(0.2,0.0,0.0,1.0) ; vertexColor=vec4(0.0,0.0,1.0,1.0);}
    else if(gl_VertexID == 2) { gl_Position = vec4(-0.2,0.4,0.0,1.0); vertexColor=vec4(0.0,0.0,1.0,1.0);}
    else if(gl_VertexID == 3) {gl_Position = vec4(0.2,0.0,0.0,1.0) ; vertexColor=vec4(1.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 4) {gl_Position = vec4(-0.2,0.4,0.0,1.0) ; vertexColor=vec4(1.0,0.0,0.0,1.0);}
    else {gl_Position = vec4(0.2,0.4,0.0,1.0); vertexColor=vec4(1.0,0.0,0.0,1.0);}

}
