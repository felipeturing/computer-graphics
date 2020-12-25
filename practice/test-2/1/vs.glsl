#version 410

layout (location=0) in vec3 position;  // coord

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform int obj;

out vec4 varyingColor;  // be interpolated by the rasterizer

void main(void) {
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);  // right-to-left
//    varyingColor = vec4(position, 1.0) * 0.5 + vec4 (0.5, 0.5, 0.5, 0.5);

/*    if(gl_VertexID < 36 ){
        varyingColor = vec4(1.0,0.0,0.0,1.0);
    }else varyingColor = vec4(0.0,1.0,0.0,1.0);
*/
    if(obj == 1 ) varyingColor = vec4(0.0,0.0,1.0,1.0);
    else if(obj == 2) varyingColor = vec4(0.0,1.0,0.0,1.0);
    else if(obj == 3) varyingColor = vec4(0.0471, 0.718, 0.949, 1.0);
    else varyingColor = vec4(1.0, 0.502, 0.0,1.0);
}
