#version 460 core

out vec4 fragcolor;

in vec3 fragpos;
in vec3 normal;
in vec2 Texcord;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform vec3 campos;

struct Light {
    vec3 dir;
};

struct Grass{
    vec3 ambient;
    vec3 difffusion;
    vec3 specular;
};


uniform Light light;

vec3 calcDirLight(vec3 fragpos, vec3 ldir, vec3 campos, vec3 norm) {
    vec3 ambient = vec3(0.1f);
    float diff = max(dot(norm, -ldir), 0.0);
    vec3 diffuse = vec3(1.f) * diff * 0.9;
    vec3 reflec = reflect(ldir, norm);
    vec3 viewdir = normalize(campos - fragpos);
    float spec = pow(max(dot(viewdir, reflec), 0.0f), 32);
    vec3 specular = vec3(0.5) * spec;
    return (ambient + diffuse + specular);
}

void main() {
    vec3 norr = normalize(normal);
    vec3 ans = calcDirLight(fragpos, light.dir, campos, norr);
    // if (normal.y > 0.5f) fragcolor = vec4(pow(vec3(texture(texture1, Texcord)) * ans, vec3(1 / 2.2f)), 1.0f);
    // else fragcolor = vec4(pow(vec3(texture(texture0, Texcord)) * ans, vec3(1 / 2.2f)), 1.0f);
    fragcolor = vec4(pow(vec3(0.26f,0.81f,0.218f) * ans, vec3(1 / 2.2f)), 1.0f);
}
