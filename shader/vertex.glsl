#version 460 core
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 norms;
layout(location = 2) in vec2 texcord;

out vec3 normal;
out vec3 fragpos;
out vec2 Texcord;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main() {
    vec4 mpos = model * vec4(position,1.0f);
    gl_Position = projection * view * mpos;
    normal = norms;
    fragpos = vec3(mpos);
    Texcord = texcord;
}
