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

vec3 calcDirLight(vec3 fragpos, vec3 ldir, vec3 campos, vec3 norm,vec3 hdr) {
    vec3 ambient = vec3(0.5f) * hdr;
    float diff = max(dot(norm, -ldir), 0.0);
    vec3 diffuse = vec3(1.f) * diff * 0.9;
    vec3 viewdir = normalize(campos - fragpos);
    vec3 halfvec = normalize(-ldir + viewdir);
    float spec = pow(max(dot(viewdir, halfvec), 0.0f), 64);
    vec3 specular = vec3(0.1) * spec;
    return (ambient + diffuse + specular);
}
const vec2 invAtan = vec2(0.1591, 0.3183); // 1/(2*PI), 1/PI
float exposure = 1.0;

vec2 SampleSphericalMap(vec3 v) {
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}
void main() {
    vec3 fragc;
    vec3 norr = normalize(normal);
    vec2 uv = SampleSphericalMap(norr);
    vec3 color = texture(texture2, uv).rgb;
    vec3 ans = calcDirLight(fragpos, light.dir, campos, norr,color);
    if (normal.y > 0.5f)
        fragc = vec3(texture(texture1, Texcord)) * ans * outAO;
    else fragc = vec3(texture(texture0, Texcord)) * ans * outAO;
    vec3 mapped = vec3(1.0) - exp(-fragc * exposure);
    mapped = pow(mapped, vec3(1.0 / 2.2));
    fragcolor = vec4(mapped, 1.0f);
}
