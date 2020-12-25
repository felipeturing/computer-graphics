#version 430

layout (location = 0) in vec3 position;
out vec4 varyingColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform int obj;
layout (binding=0) uniform sampler2D s;

void main(void)
{
    gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
	if(obj == 1 ) varyingColor = vec4(22.0/255.0,42.0/255.0,0.0,1.0);
    else varyingColor = vec4(13.0/255.0,17.0/255.0,22.0/255.0,1.0);

}
