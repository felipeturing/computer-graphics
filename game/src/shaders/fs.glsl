#version 430

in vec3 varyingNormal, varyingLightDir, varyingVertPos, varyingHalfVec;
in vec3 varyingTangent;
in vec3 originalVertex;
in vec4 shadow_coord;
in vec2 tc;

out vec4 fragcolor;

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
uniform mat4 norm_matrix;
uniform mat4 shadowMVP;
uniform int obj;

layout (binding=0) uniform sampler2DShadow shadowTex;
layout (binding=1) uniform sampler2D s;
layout (binding=2) uniform sampler2D normMap;


vec3 calcNewNormal()
{
	vec3 normal = normalize(varyingNormal);
	vec3 tangent = normalize(varyingTangent);
	tangent = normalize(tangent - dot(tangent, normal) * normal);
	vec3 bitangent = cross(tangent, normal);
	mat3 tbn = mat3(tangent, bitangent, normal);
	vec3 retrievedNormal = texture(normMap,tc).xyz;
	retrievedNormal = retrievedNormal * 2.0 - 1.0;
	vec3 newNormal = tbn * retrievedNormal;
	newNormal = normalize(newNormal);
	return newNormal;
}

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
	vec3 N = calcNewNormal();
	vec3 H = normalize(varyingHalfVec);

    /*float swidth = 2.5;
	vec2 o = mod(floor(gl_FragCoord.xy), 2.0) * swidth;
	shadowFactor += lookup(-1.5*swidth + o.x,  1.5*swidth - o.y);
	shadowFactor += lookup(-1.5*swidth + o.x, -0.5*swidth - o.y);
	shadowFactor += lookup( 0.5*swidth + o.x,  1.5*swidth - o.y);
	shadowFactor += lookup( 0.5*swidth + o.x, -0.5*swidth - o.y);
	shadowFactor = shadowFactor / 4.0;*/

    //shadowFactor = lookup(0.0, 0.0);
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

    vec4 shadowColor = globalAmbient * material.ambient
				+ light.ambient * material.ambient;
    vec4 lightedColor = light.diffuse * material.diffuse * max(dot(L,N),0.0)
				+ light.specular * material.specular
				* pow(max(dot(H,N),0.0),material.shininess*3.0);


    if (obj==43){ // human body
        color= vec4(0.55,0.0,0.55,1.0);
    }else{
	    color = texture(s,tc);
    }


    fragcolor = vec4((shadowColor.xyz + shadowFactor*(0.5*lightedColor.xyz+ 0.5*color.xyz)),1.0);
    //fragcolor = 0.5*lightedColor + 0.5*texture(s,tc);
}




/* //versión antigua de fragment shader para la primera presentación
#version 430

in vec2 tc;
out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform int obj;
layout (binding=0) uniform sampler2D s;

void main(void)
{
    if (obj >= 40 && obj <= 49 ){ // human body
        if(obj == 41) color = vec4(0.0,0.0,0.0,1.0); //head
        else if (obj == 42) color = vec4(0.0,0.0,0.55,1.0); // clothing t-shirt and shoes and hands
        else if (obj == 43) color = vec4(255.0/255.0,224.0/255.0,189.0/255.0,1.0); //skin
        else if (obj == 44) color = vec4(0.55,0.0,0.55,1.0); //clothing shorts
        //else if (obj == 45)
        //else if (obj == 46)
        else color = vec4(255.0/255.0,224.0/255.0,189.0/255.0,1.0);
    }else if(obj == 5){
        color = vec4(0.5,0.5,0.5,1.0);
    }else{
	    color = texture(s,tc);
    }
}

*/
