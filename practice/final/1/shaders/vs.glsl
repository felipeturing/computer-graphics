#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;


out vec3 varyingNormal, varyingLightDir, varyingVertPos, varyingHalfVec,originalVertex;
out vec4 shadow_coord;


struct PositionalLight
{	vec4 ambient, diffuse, specular;
	vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};
struct Material
{	vec4 ambient, diffuse, specular;
	float shininess;
};

uniform vec4 globalAmbient;
uniform PositionalLight light;
uniform Material material;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform int obj;
uniform mat4 norm_matrix;
uniform mat4 shadowMVP;

layout(binding=0) uniform sampler2DShadow shadowTex;

void main(void){
    varyingVertPos = (mv_matrix * vec4(vertPos,1.0)).xyz; //vertice rasterizado por interpolacion
    varyingLightDir = light.position - varyingVertPos;//el vector de la luz a la posi rasterizado por interpolacion
    varyingNormal = (norm_matrix * vec4(vertNormal,1.0)).xyz;//normal rasterizada
    varyingHalfVec = normalize(normalize(varyingLightDir) + normalize(-varyingVertPos)).xyz;
    shadow_coord = shadowMVP * vec4(vertPos,1.0);
    originalVertex = vertPos;
    gl_Position = proj_matrix * mv_matrix * vec4(vertPos,1.0);
}
