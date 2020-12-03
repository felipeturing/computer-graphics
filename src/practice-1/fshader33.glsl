#version 410
//in vec4 vertexColor;
layout (location = 0) in vec4 vertexColor;
out vec4 outColor;

void main(void){
    outColor = vec4(vertexColor);

}

/**#version 410
//layout (location = 0) out vec4 vertexColor;
//uniform vec2 resolution;
//out vec4 vertexColor;
void main(void){
    vec2 uv = gl_FragCoord.xy;
    gl_FragColor = vec4(uv.x,uv.y,0.0,1.0);
}**/
