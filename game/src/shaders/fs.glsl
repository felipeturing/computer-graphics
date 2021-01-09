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
