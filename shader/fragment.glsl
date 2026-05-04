#version 460 core
out vec4 fragcolor;

in vec3 normal;

void main(){
    fragcolor = vec4(abs(normal),1.0f);
}
