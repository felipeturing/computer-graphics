#version 430

out vec4 color;
in vec4 varyingColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

layout (binding=0) uniform sampler2D s;

void main(void)
{
	color = varyingColor;
}
