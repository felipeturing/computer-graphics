#version 430

in vec3 vNormal;
in vec3 vVertPos;
out vec4 fragColor;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform mat4 normalMat;
layout (binding = 0) uniform samplerCube t;

void main(void)
{
	vec3 r = -reflect(normalize(-vVertPos), normalize(vNormal));
	//vec4 tinte = vec4(1.0f,0.0f,0.0f,0.0f);
	fragColor = texture(t,r*0.5 + vec3(0.5f,0.3f,0.5f) );
}
