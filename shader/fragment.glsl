#version 460 core

out vec4 fragcolor;

in float outAO;
in vec3 fragpos;
in vec3 normal;
in vec2 Texcord;
uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform vec3 campos;

struct Light {
    vec3 dir;
};

struct Grass {
    vec3 ambient;
    vec3 difffusion;
    vec3 specular;
};

uniform Light light;

vec3 calcDirLight(vec3 fragpos, vec3 ldir, vec3 campos, vec3 norm) {
    vec3 ambient = vec3(0.2f);
    float diff = max(dot(norm, -ldir), 0.0);
    vec3 diffuse = vec3(1.f) * diff * 0.9;
    vec3 viewdir = normalize(campos - fragpos);
    vec3 halfvec = normalize(-ldir + viewdir);
    float spec = pow(max(dot(viewdir, halfvec), 0.0f), 64);
    vec3 specular = vec3(0.1) * spec;
    return (ambient + diffuse + specular);
}

void main() {
    // vec3 norr = normalize(vec3(texture(texture2, Texcord)) * 2.f - 1.f);
    vec3 norr = normalize(normal);
    vec3 ans = calcDirLight(fragpos, light.dir, campos, norr);
    if (normal.y > 0.5f)
        fragcolor = vec4(pow(vec3(texture(texture1, Texcord)) * ans , vec3(1 / 2.2f)), 1.0f) * outAO ;
    else fragcolor = vec4(pow(vec3(texture(texture0, Texcord)) * ans * outAO, vec3(1 / 2.2f)), 1.0f);
    // fragcolor = vec4(pow(vec3(0.26f,0.81f,0.218f) * ans, vec3(1 / 2.2f)), 1.0f);
}
