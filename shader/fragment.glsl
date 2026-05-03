#version 460 core
out vec4 fragcolor;

in vec3 apos;

void main(){
    fragcolor = vec4(apos,1.0f);
}
