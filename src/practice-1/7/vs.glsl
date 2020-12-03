#version 410
layout (location = 0) out vec4 vertexColor; // Se envia el vertice al Fragment Shader
uniform float offset; // capturador de la variable externa offset
void main(void){
    if(gl_VertexID == 0){
        gl_Position = vec4(-0.25 + offset,0.0,0.0,1.0);
        //vertexColor = vec4(1.0,0.0,0.0,1.0);
    }else if(gl_VertexID == 1){
        gl_Position = vec4(0.25 + offset,0.0,0.0,1.0) ;
        //vertexColor = vec4(0.0,1.0,0.0,1.0);
    }else{
        gl_Position = vec4(offset,0.25,0.0,1.0);
        //vertexColor = vec4(0.0,0.0,1.0,1.0);
    }
    vertexColor = gl_Position;
}
