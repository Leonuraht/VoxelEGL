#version 460 core
out vec4 fragcolor;

in vec3 normal;
uniform sampler2D textdata;

void main(){
    fragcolor = vec4(abs(normal),1.0f);
}
