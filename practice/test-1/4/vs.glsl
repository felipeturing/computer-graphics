#version 410
#define M_PI 3.14159265358
layout (location = 0) out vec4 vertexColor; // Se envia el vertice al Fragment Shader
mat4 cizallarZ(float lambda){
    mat4 cZ = mat4(1.0,  0.0, 0.0, 0.0,
                   0.0, 1.0, 0.0, lambda,
                   0.0, 0.0, 1.0, 0.0,
                   0.0, 0.0, 0.0, 1.0);
    return cZ;
}
void main(void){
    vec4 vertexPos;
    if(gl_VertexID == 0) { vertexPos = vec4(-0.5,-0.5,0.0,1.0); vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 1)  { vertexPos = vec4(0.0,-0.5,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 2)  { vertexPos = vec4(0.0,0.0,0.0,1.0); vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 3)  { vertexPos = vec4(-0.5,0.0,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 4)  { vertexPos = vec4(0.0,0.0,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 5)  { vertexPos = vec4(-0.5,-0.5,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}

    else if(gl_VertexID == 6)  { vertexPos = vec4(0.5,-0.5,0.0,1.0); vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 7)  { vertexPos = vec4(0.0,-0.5,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 8)  { vertexPos = vec4(0.0,0.0,0.0,1.0); vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 9)  { vertexPos = vec4(0.5,0.0,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 10) { vertexPos = vec4(0.0,0.0,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 11) { vertexPos = vec4(0.5,-0.5,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}

    else if(gl_VertexID == 12) { vertexPos = vec4(0.0,0.0,0.0,1.0); vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 13) { vertexPos = vec4(0.5,0.0,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 14) { vertexPos = vec4(0.5,0.5,0.0,1.0); vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 15) { vertexPos = vec4(0.0,0.5,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 16) { vertexPos = vec4(0.0,0.0,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}
    else if(gl_VertexID == 17) { vertexPos = vec4(0.5,0.5,0.0,1.0) ; vertexColor=vec4(1.0,1.0,1.0,1.0);}

    else if(gl_VertexID == 18) { vertexPos = vec4(0.0,0.0,0.0,1.0); vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 19) { vertexPos = vec4(-0.5,0.0,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 20) { vertexPos = vec4(-0.5,0.5,0.0,1.0); vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 21) { vertexPos = vec4(0.0,0.5,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else if(gl_VertexID == 22) { vertexPos = vec4(0.0,0.0,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}
    else  { vertexPos = vec4(-0.5,0.5,0.0,1.0) ; vertexColor=vec4(0.0,0.0,0.0,1.0);}

    gl_Position = cizallarZ(1.0f)*vertexPos;
}
