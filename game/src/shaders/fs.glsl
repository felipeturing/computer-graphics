#version 430

in vec2 tc;
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform int obj;
layout (binding=0) uniform sampler2D s;

void main(void)
{
    if (obj == 4){
        color = vec4(0.0,0.0,1.0,1.0);
    }else{
	    color = texture(s,tc);
    }
}
