#version 430

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 vertNormal;
layout (location = 2) in vec2 tex_coord;

out vec3 varyingNormal, varyingLightDir, varyingVertPos, varyingHalfVec;
out vec4 shadow_coord;
out vec2 tc;


struct PositionalLight
{	vec4 ambient, diffuse, specular;
	vec3 position;
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
layout(binding=1) uniform sampler2D s;

void main(void){
    vec3 newVertPos;
    if(obj == 1){// la roca
        newVertPos = vertPos + vec3(0.0,0.0,-180.0*(gl_InstanceID));
    }else{
        newVertPos = vertPos;
    }

    varyingVertPos = (mv_matrix * vec4(newVertPos,1.0)).xyz; //vertice rasterizado por interpolacion
    varyingLightDir = light.position - varyingVertPos;//el vector de la luz a la posi rasterizado por interpolacion
    varyingNormal = (norm_matrix * vec4(vertNormal,1.0)).xyz;//normal rasterizada
    varyingHalfVec = (varyingLightDir-varyingVertPos).xyz;//halfvector  L + V

    shadow_coord = shadowMVP * vec4(newVertPos,1.0);

    gl_Position = proj_matrix * mv_matrix * vec4(newVertPos,1.0);
	tc = tex_coord;
}


/* //versión anterio del vertex shader para la primera presentación
#version 430

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coord;

out vec2 tc;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform int obj;

layout (binding=0) uniform sampler2D s;

void main(void)
{
    if(obj == 1){// la roca
        vec3 newposition;
        newposition = position + vec3(0.0,0.0,-180.0*(gl_InstanceID));
        gl_Position = proj_matrix * mv_matrix * vec4(newposition,1.0);
    }else{
        gl_Position = proj_matrix * mv_matrix * vec4(position,1.0);
    }
	tc = tex_coord;
}
*/
