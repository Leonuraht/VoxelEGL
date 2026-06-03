#version 460 core
layout(location = 0) in uint data;

out vec3 normal;
out vec3 fragpos;
out vec2 Texcord;
out float outAO;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec2 chunk_cord;

const vec3 faceVertices[6][6] = vec3[6][6](
        // 0: RIGHT (+X)
        vec3[6](vec3(0.5, -0.5, 0.5), vec3(0.5, -0.5, -0.5), vec3(0.5, 0.5, -0.5), vec3(0.5, 0.5, -0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, -0.5, 0.5)),
        // 1: LEFT (-X)
        vec3[6](vec3(-0.5, -0.5, -0.5), vec3(-0.5, -0.5, 0.5), vec3(-0.5, 0.5, 0.5), vec3(-0.5, 0.5, 0.5), vec3(-0.5, 0.5, -0.5), vec3(-0.5, -0.5, -0.5)),
        // 2: TOP (+Y)
        vec3[6](vec3(-0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, -0.5), vec3(0.5, 0.5, -0.5), vec3(-0.5, 0.5, -0.5), vec3(-0.5, 0.5, 0.5)),
        // 3: BOTTOM (-Y)
        vec3[6](vec3(-0.5, -0.5, -0.5), vec3(0.5, -0.5, -0.5), vec3(0.5, -0.5, 0.5), vec3(0.5, -0.5, 0.5), vec3(-0.5, -0.5, 0.5), vec3(-0.5, -0.5, -0.5)),
        // 4: FRONT (+Z)
        vec3[6](vec3(-0.5, -0.5, 0.5), vec3(0.5, -0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(0.5, 0.5, 0.5), vec3(-0.5, 0.5, 0.5), vec3(-0.5, -0.5, 0.5)),
        // 5: BACK (-Z)
        vec3[6](vec3(0.5, -0.5, -0.5), vec3(-0.5, -0.5, -0.5), vec3(-0.5, 0.5, -0.5), vec3(-0.5, 0.5, -0.5), vec3(0.5, 0.5, -0.5), vec3(0.5, -0.5, -0.5))
    );

const vec3 faceNormals[6] = vec3[6](
        vec3(1.0, 0.0, 0.0), // 0: RIGHT
        vec3(-1.0, 0.0, 0.0), // 1: LEFT
        vec3(0.0, 1.0, 0.0), // 2: TOP
        vec3(0.0, -1.0, 0.0), // 3: BOTTOM
        vec3(0.0, 0.0, 1.0), // 4: FRONT
        vec3(0.0, 0.0, -1.0) // 5: BACK
    );

const vec2 faceUVs[6] = vec2[6](
        vec2(0.0, 0.0), vec2(1.0, 0.0), vec2(1.0, 1.0),
        vec2(1.0, 1.0), vec2(0.0, 1.0), vec2(0.0, 0.0)
    );
void main() {
    float posx = chunk_cord.x + (data & 0x1Fu);
    float posy = ((data >> 5u) & 0xFFu) - 86.f;
    float posz = chunk_cord.y + ((data >> 13u) & 0x1Fu);
    uint face_num = (data >> 18u) & 0x7u;
    uint ao0 = (data >> 21u) & 0x3u;
    uint ao1 = (data >> 23u) & 0x3u;
    uint ao2 = (data >> 25u) & 0x3u;
    uint ao3 = (data >> 27u) & 0x3u;

    uint aoValues[6] = uint[6](ao1, ao2, ao3, ao3, ao0, ao1);
    uint vertexAO = aoValues[gl_VertexID];
    float aoFactor = 0.4 + (float(vertexAO) / 3.0) * 0.6;
    outAO = aoFactor;
    vec3 position = vec3(posx, posy, posz) + faceVertices[face_num][gl_VertexID];
    vec4 mpos = model * vec4(position, 1.0f);
    gl_Position = projection * view * mpos;
    normal = faceNormals[face_num];
    fragpos = vec3(mpos);
    Texcord = faceUVs[gl_VertexID];
}
