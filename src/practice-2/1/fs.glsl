/**#version 410
//in vec4 vertexColor;
layout (location = 0) in vec4 vertexColor;
out vec4 outColor;

void main(void){
    outColor = vec4(vertexColor);

}**/

#version 410
void main(void){
    /**
    * gl_FragCoord coordinates are indexed starting from 0.
    * The expression gl_FragCoord.x - 5 would result in a value in the range [-5, (width - 5) - 1]
    **/
    if(gl_FragCoord.x<=400){
        gl_FragColor = vec4(1.0,0.0,0.0,1.0);
    }else{
        gl_FragColor = vec4(0.0,0.0,1.0,1.0);
    }
}
