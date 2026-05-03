#version 460 core
layout(location = 0) in vec3 position;

out vec3 apos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main(){
    gl_Position = vec4(position,1.0f);
    apos = position + 0.2f;
}
