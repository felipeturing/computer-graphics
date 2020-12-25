#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;
out vec2 tc;

//out int flag;
//out vec4 varyingColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
layout (binding=0) uniform sampler2D s;

void main(void)
{	gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	tc = tex_coord;

    //varyingColor = vec4(1.0,0.0,0.0,1.0);

    /*if (gl_VertexID < 48*48*6){
        flag = 1; //tierra
    }else{
        flag = 2; //objeto caedor
    }*/
}
