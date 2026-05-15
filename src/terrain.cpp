#include "Misc/terrain.hpp"
#include <GLFW/glfw3.h>
#include <cstdint>
#include <cstdlib>
// #include <iostream>
int yl = 256;

Chunk::Chunk(int x, int z) {
    this->x = x;
    this->z = z;
}

void add_faces(Vertex *mesh_buffer, int &v_count, Chunk &chunk, uint8_t *data);

std::vector<Vertex> generatefaces(uint8_t *data, Chunk &chunk,
                                  int threadchunks) {
    Vertex *raw_buff = new Vertex[100000];
    int vert_c = 0;
    add_faces(raw_buff, vert_c, chunk, data);
    std::vector<Vertex> mesh;
    mesh.reserve(vert_c);
    mesh.assign(raw_buff, raw_buff + vert_c);

    delete[] raw_buff;
    freedata(data);
    return mesh;
}

std::vector<std::vector<float>> heightmap;

const int p[512] = {
    151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233, 7,
    225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,  190,
    6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203, 117,
    35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125, 136,
    171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146, 158,
    231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,  46,
    245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,  209,
    76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159, 86,
    164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123, 5,
    202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,  16,
    58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,   44,
    154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,  253,
    19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246, 97,
    228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,  51,
    145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157, 184,
    84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205, 93,
    222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,  156,
    180, 151, 160, 137, 91,  90,  15,  131, 13,  201, 95,  96,  53,  194, 233,
    7,   225, 140, 36,  103, 30,  69,  142, 8,   99,  37,  240, 21,  10,  23,
    190, 6,   148, 247, 120, 234, 75,  0,   26,  197, 62,  94,  252, 219, 203,
    117, 35,  11,  32,  57,  177, 33,  88,  237, 149, 56,  87,  174, 20,  125,
    136, 171, 168, 68,  175, 74,  165, 71,  134, 139, 48,  27,  166, 77,  146,
    158, 231, 83,  111, 229, 122, 60,  211, 133, 230, 220, 105, 92,  41,  55,
    46,  245, 40,  244, 102, 143, 54,  65,  25,  63,  161, 1,   216, 80,  73,
    209, 76,  132, 187, 208, 89,  18,  169, 200, 196, 135, 130, 116, 188, 159,
    86,  164, 100, 109, 198, 173, 186, 3,   64,  52,  217, 226, 250, 124, 123,
    5,   202, 38,  147, 118, 126, 255, 82,  85,  212, 207, 206, 59,  227, 47,
    16,  58,  17,  182, 189, 28,  42,  223, 183, 170, 213, 119, 248, 152, 2,
    44,  154, 163, 70,  221, 153, 101, 155, 167, 43,  172, 9,   129, 22,  39,
    253, 19,  98,  108, 110, 79,  113, 224, 232, 178, 185, 112, 104, 218, 246,
    97,  228, 251, 34,  242, 193, 238, 210, 144, 12,  191, 179, 162, 241, 81,
    51,  145, 235, 249, 14,  239, 107, 49,  192, 214, 31,  181, 199, 106, 157,
    184, 84,  204, 176, 115, 121, 50,  45,  127, 4,   150, 254, 138, 236, 205,
    93,  222, 114, 67,  29,  24,  72,  243, 141, 128, 195, 78,  66,  215, 61,
    156, 180};

float fade(float t) { return t * t * t * (t * (t * 6.0f - 15.0f) + 10.0f); }
float lerp(float t, float a, float b) { return a + t * (b - a); }
float grad(int hash, float x, float z) {
    int h = hash & 15;
    float u = h < 8 ? x : z;
    float v = h < 4 ? z : h == 12 || h == 14 ? x : 0.0f;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

float perlin2D(float x, float z) {
    int X = (int)floor(x) & 255;
    int Z = (int)floor(z) & 255;

    x -= floor(x);
    z -= floor(z);

    float u = fade(x);
    float v = fade(z);

    int A = p[X] + Z;
    int B = p[X + 1] + Z;

    return lerp(v, lerp(u, grad(p[A], x, z), grad(p[B], x - 1.0f, z)),
                lerp(u, grad(p[A + 1], x, z - 1.0f),
                     grad(p[B + 1], x - 1.0f, z - 1.0f)));
}
void generateheightmap(Chunk &chunk) {
    for (int i = 0; i < 18; i++) {
        for (int j = 0; j < 18; j++) {
            float noise =
                perlin2D(i * 0.025 + chunk.x * 0.4, j * 0.025 + chunk.z * 0.4) *
                33;
            chunk.heightmap[i + j * 18] = (int)noise + yl / 4;
            // std::cout << (int)noise + 40 << std::endl;
        }
    }
}

uint8_t *generateWorld(Chunk &chunk, int threadchunks) {
    uint8_t *blockdata = (uint8_t *)calloc(16 * 16 * yl, sizeof(uint8_t));
    generateheightmap(chunk);
    int h = yl * 16;
    for (int i = 0; i < 16; i++) {
        for (int k = 0; k < 16; k++) {
            // for(int j = 0;j < 4;j++) blockdata[i * h + j * 16 + k] = 2;
            int height = chunk.heightmap[(i + 1) + (k + 1) * 18];
            height = (height > yl) ? yl : height;
            for (int j = 0; j < height; j++) {
                blockdata[i * h + j * 16 + k] = 1;
            }
        }
    }
    return blockdata;
}

void freedata(uint8_t *mat) { free(mat); }

void add_faces(Vertex *mesh_buffer, int &v_count, Chunk &chunk, uint8_t *data) {
    int h = yl * 16;
    for (int i = 0; i < 16; i++) {
        for (int j = 0; j < yl; j++) {
            for (int k = 0; k < 16; k++) {
                int inde = i * h + j * 16 + k;
                if (data[inde] <= 0)
                    continue;

                int hIdx = (i + 1) + (k + 1) * 18;

                if (i == 15 && j < chunk.heightmap[hIdx + 1]) {
                } else if (i == 15 || data[inde + h] == 0) {
                    uint32_t packedFace = 0;
                    packedFace |= (i & 0x1F);
                    packedFace |= ((j & 0xFF) << 5);
                    packedFace |= ((k & 0x1F) << 13);
                    packedFace |= ((0 & 0x7) << 18);
                    mesh_buffer[v_count++].data = packedFace;
                }

                if (i == 0 && j < chunk.heightmap[hIdx - 1]) {
                } else if (i == 0 || data[inde - h] == 0) {
                    uint32_t packedFace = 0;
                    packedFace |= (i & 0x1F);
                    packedFace |= ((j & 0xFF) << 5);
                    packedFace |= ((k & 0x1F) << 13);
                    packedFace |= ((1 & 0x7) << 18);
                    mesh_buffer[v_count++].data = packedFace;
                }

                // TOP FACE
                if (j == yl - 1 || data[inde + 16] == 0) {
                    uint32_t packedFace = 0;
                    packedFace |= (i & 0x1F);
                    packedFace |= ((j & 0xFF) << 5);
                    packedFace |= ((k & 0x1F) << 13);
                    packedFace |= ((2 & 0x7) << 18);
                    mesh_buffer[v_count++].data = packedFace;
                }

                // FRONT FACE
                if (k == 15 && j < chunk.heightmap[hIdx + 18]) {
                } else if (k == 15 || data[inde + 1] == 0) {
                    uint32_t packedFace = 0;
                    packedFace |= (i & 0x1F);
                    packedFace |= ((j & 0xFF) << 5);
                    packedFace |= ((k & 0x1F) << 13);
                    packedFace |= ((4 & 0x7) << 18);
                    mesh_buffer[v_count++].data = packedFace;
                }
                // BACK FACE
                if (k == 0 && j < chunk.heightmap[hIdx - 18]) {
                } else if (k == 0 || data[inde - 1] == 0) {
                    uint32_t packedFace = 0;
                    packedFace |= (i & 0x1F);
                    packedFace |= ((j & 0xFF) << 5);
                    packedFace |= ((k & 0x1F) << 13);
                    packedFace |= ((5 & 0x7) << 18);
                    mesh_buffer[v_count++].data = packedFace;
                }

                // BOTTOM FACE
                if (j != 0 && data[inde - 16] == 0) {
                    uint32_t packedFace = 0;
                    packedFace |= (i & 0x1F);
                    packedFace |= ((j & 0xFF) << 5);
                    packedFace |= ((k & 0x1F) << 13);
                    packedFace |= ((3 & 0x7) << 18);
                    mesh_buffer[v_count++].data = packedFace;
                }
            }
        }
    }
}
