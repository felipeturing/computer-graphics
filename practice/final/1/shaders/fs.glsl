#version 430

in vec3 varyingNormal, varyingLightDir, varyingVertPos, varyingHalfVec;
in vec3 originalVertex;
in vec4 shadow_coord;


out vec4 fragcolor;

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
uniform mat4 norm_matrix;
uniform mat4 shadowMVP;
uniform int obj;

layout (binding=0) uniform sampler2DShadow shadowTex;


float lookup(float x, float y)
{  	float t = textureProj(shadowTex, shadow_coord + vec4(x * 0.001 * shadow_coord.w,
                                                         y * 0.001 * shadow_coord.w,
                                                         -0.01, 0.0));
	return t;
}

void main(void){
    float shadowFactor=0.0;
    vec4 color;
	vec3 L = normalize(varyingLightDir);
    vec3 V = normalize(-varyingVertPos);
	vec3 N = normalize(varyingNormal);
	vec3 H = normalize(varyingHalfVec);

    /*float swidth = 2.5;
	vec2 o = mod(floor(gl_FragCoord.xy), 2.0) * swidth;
	shadowFactor += lookup(-1.5*swidth + o.x,  1.5*swidth - o.y);
	shadowFactor += lookup(-1.5*swidth + o.x, -0.5*swidth - o.y);
	shadowFactor += lookup( 0.5*swidth + o.x,  1.5*swidth - o.y);
	shadowFactor += lookup( 0.5*swidth + o.x, -0.5*swidth - o.y);
	shadowFactor = shadowFactor / 4.0;*/

    // hi res PCF
    /*float width = 2.5;
	float endp = width * 3.0 + width/2.0;
	for (float m=-endp ; m<=endp ; m=m+width)
	{	for (float n=-endp ; n<=endp ; n=n+width)
		{	shadowFactor += lookup(m,n);
	}	}
	shadowFactor = shadowFactor / 64.0;*/

	// this would produce normal hard shadows

    shadowFactor = lookup(0.0, 0.0);
    vec4 ambient = light.ambient * material.diffuse;
    vec4 diffuse = light.diffuse * material.diffuse * max(dot(L,N),0.0);
    vec4 specular = light.specular * material.specular * pow(max(dot(H,N),0.0),material.shininess*3.0);


    // spotlight (soft edges)
    float theta = dot(L, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse.xyz  *= intensity;
    specular.xyz *= intensity;

    // attenuation
    /*float distance    = length(light.position - originalVertex);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient.xyz  *= attenuation;
    diffuse.xyz   *= attenuation;
    specular.xyz *= attenuation;*/

    vec4 shadowColor = globalAmbient * material.ambient
				+ light.ambient * material.ambient;
    vec4 lightedColor =ambient +  diffuse +  specular;

   if (obj==1){//esfera
        color = vec4(0.0,0.0,1.0,1.0);
   }else if(obj==0){//plano
	    color = vec4(1.0,1.0,1.0,1.0);
   }

    fragcolor = vec4((shadowColor.xyz + shadowFactor*(0.5*lightedColor.xyz+ 0.5*color.xyz)),1.0);

}
