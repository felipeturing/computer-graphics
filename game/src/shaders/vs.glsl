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
