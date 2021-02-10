#version 430

layout (location=0) in vec3 position;

uniform mat4 shadowMVP;
uniform int obj;

void main(void){
	//gl_Position = shadowMVP * vec4(position,1.0);

    if(obj == 1){// la roca
        vec3 newposition;
        newposition = position + vec3(0.0,0.0,-180.0*(gl_InstanceID));
        gl_Position = shadowMVP*vec4(newposition,1.0);
    }else{
        gl_Position = shadowMVP*vec4(position,1.0);
    }
}
