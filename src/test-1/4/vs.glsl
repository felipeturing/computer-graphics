#version 410
#define M_PI 3.14159265358
layout (location = 0) out vec4 vertexColor; // Se envia el vertice al Fragment Shader
void main(void){

    if(gl_VertexID == 0) { gl_Position = vec4(-0.75,-0.5,0.0,1.0); vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 1)  { gl_Position = vec4(0.0,-0.5,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 2)  { gl_Position = vec4(0.0,0.0,0.0,1.0); vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 3)  { gl_Position = vec4(-0.60,0.0,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 4)  { gl_Position = vec4(0.0,0.0,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 5)  { gl_Position = vec4(-0.75,-0.5,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}

    else if(gl_VertexID == 6)  { gl_Position = vec4(0.75,-0.5,0.0,1.0); vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 7)  { gl_Position = vec4(0.0,-0.5,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 8)  { gl_Position = vec4(0.0,0.0,0.0,1.0); vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 9)  { gl_Position = vec4(0.6,0.0,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 10) { gl_Position = vec4(0.0,0.0,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 11) { gl_Position = vec4(0.75,-0.5,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}

    else if(gl_VertexID == 12) { gl_Position = vec4(0.0,0.0,0.0,1.0); vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 13) { gl_Position = vec4(0.6,0.0,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 14) { gl_Position = vec4(0.5,0.5,0.0,1.0); vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 15) { gl_Position = vec4(0.0,0.5,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 16) { gl_Position = vec4(0.0,0.0,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 17) { gl_Position = vec4(0.5,0.5,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}

    else if(gl_VertexID == 18) { gl_Position = vec4(0.0,0.0,0.0,1.0); vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 19) { gl_Position = vec4(-0.6,0.0,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 20) { gl_Position = vec4(-0.5,0.5,0.0,1.0); vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 21) { gl_Position = vec4(0.0,0.5,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 22) { gl_Position = vec4(0.0,0.0,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else  { gl_Position = vec4(-0.5,0.5,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
}
