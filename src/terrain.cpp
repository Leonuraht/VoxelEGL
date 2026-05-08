#include "Misc/terrain.hpp"
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>

int xl = 100, yl = 100, zl = 100;


float frontface[] = {
    // --- Front Face (Z = +0.5) ---          // UV Coordinates
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-Left
    0.5f,  -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // Bottom-Right
    0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Top-Right
    0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Top-Right
    -0.5f, 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // Top-Left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // Bottom-Left
};

float backface[] = {
    // --- Back Face (Z = -0.5) ---           // UV Coordinates
    0.5f,  -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-Left
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // Bottom-Right
    -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // Top-Right
    -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // Top-Right
    0.5f,  0.5f,  -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // Top-Left
    0.5f,  -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-Left
};

float leftface[] = {
    // --- Left Face (X = -0.5) ---           // UV Coordinates
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-Left
    -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-Right
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-Right
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-Right
    -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-Left
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-Left
};

float rightface[] = {
    // --- Right Face (X = +0.5) ---          // UV Coordinates
    0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-Left
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // Bottom-Right
    0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-Right
    0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // Top-Right
    0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // Top-Left
    0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Bottom-Left
};

float topface[] = {
    // --- Top Face (Y = +0.5) ---            // UV Coordinates
    -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Bottom-Left
    0.5f,  0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Bottom-Right
    0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Top-Right
    0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // Top-Right
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // Top-Left
    -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // Bottom-Left
};

float bottomface[] = {
    // --- Bottom Face (Y = -0.5) ---         // UV Coordinates
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // Bottom-Left
    0.5f,  -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // Bottom-Right
    0.5f,  -0.5f, 0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // Top-Right
    0.5f,  -0.5f, 0.5f,  0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // Top-Right
    -0.5f, -0.5f, 0.5f,  0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // Top-Left
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f  // Bottom-Left
};

std::vector<float> generatefaces(int ***data) {
    std::vector<float> mesh;
    mesh.reserve(500000);
    for (int i = 0; i < xl; i++) {
        for (int j = 0; j < yl; j++) {
            for (int k = 0; k < zl; k++) {
                if (data[i][j][k] <= 0)
                    continue;
                float xoff = i - xl / 2.f, yoff = j - yl / 2.f,
                      zoff = k - zl / 2.f;

                if (i == xl - 1 || data[i + 1][j][k] == 0) {
                    for (int v = 0; v < 48; v += 8) {
                        mesh.push_back(rightface[v] + xoff);
                        mesh.push_back(rightface[v + 1] + yoff);
                        mesh.push_back(rightface[v + 2] + zoff);
                        mesh.push_back(rightface[v + 3]);
                        mesh.push_back(rightface[v + 4]);
                        mesh.push_back(rightface[v + 5]);
                        mesh.push_back(rightface[v + 6]);
                        mesh.push_back(rightface[v + 7]);
                    }
                }
                if (i == 0 || data[i - 1][j][k] == 0) {
                    for (int v = 0; v < 48; v += 8) {
                        mesh.push_back(leftface[v] + xoff);
                        mesh.push_back(leftface[v + 1] + yoff);
                        mesh.push_back(leftface[v + 2] + zoff);
                        mesh.push_back(leftface[v + 3]);
                        mesh.push_back(leftface[v + 4]);
                        mesh.push_back(leftface[v + 5]);
                        mesh.push_back(leftface[v + 6]);
                        mesh.push_back(leftface[v + 7]);
                    }
                }

                if (k == zl - 1 || data[i][j][k + 1] == 0) {
                    for (int v = 0; v < 48; v += 8) {
                        mesh.push_back(frontface[v] + xoff);
                        mesh.push_back(frontface[v + 1] + yoff);
                        mesh.push_back(frontface[v + 2] + zoff);
                        mesh.push_back(frontface[v + 3]);
                        mesh.push_back(frontface[v + 4]);
                        mesh.push_back(frontface[v + 5]);
                        mesh.push_back(frontface[v + 6]);
                        mesh.push_back(frontface[v + 7]);
                    }
                }

                if (k == 0 || data[i][j][k - 1] == 0) {
                    for (int v = 0; v < 48; v += 8) {
                        mesh.push_back(backface[v] + xoff);
                        mesh.push_back(backface[v + 1] + yoff);
                        mesh.push_back(backface[v + 2] + zoff);
                        mesh.push_back(backface[v + 3]);
                        mesh.push_back(backface[v + 4]);
                        mesh.push_back(backface[v + 5]);
                        mesh.push_back(backface[v + 6]);
                        mesh.push_back(backface[v + 7]);
                    }
                }

                if (j == yl - 1 || data[i][j + 1][k] == 0) {
                    for (int v = 0; v < 48; v += 8) {
                        mesh.push_back(topface[v] + xoff);
                        mesh.push_back(topface[v + 1] + yoff);
                        mesh.push_back(topface[v + 2] + zoff);
                        mesh.push_back(topface[v + 3]);
                        mesh.push_back(topface[v + 4]);
                        mesh.push_back(topface[v + 5]);
                        mesh.push_back(topface[v + 6]);
                        mesh.push_back(topface[v + 7]);
                    }
                }

                if (j == 0 || data[i][j - 1][k] == 0) {
                    for (int v = 0; v < 48; v += 8) {
                        mesh.push_back(bottomface[v] + xoff);
                        mesh.push_back(bottomface[v + 1] + yoff);
                        mesh.push_back(bottomface[v + 2] + zoff);
                        mesh.push_back(bottomface[v + 3]);
                        mesh.push_back(bottomface[v + 4]);
                        mesh.push_back(bottomface[v + 5]);
                        mesh.push_back(bottomface[v + 6]);
                        mesh.push_back(bottomface[v + 7]);
                    }
                }
            }
        }
    }
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
void generateheightmap() {
    for (int i = 0; i < xl; i++) {
        std::vector<float> rows;
        float min = FLT_MAX;
        for (int j = 0; j < zl; j++) {
            float height = perlin2D((i * 0.025), (j * 0.015)) * 87;
            if (height < min)
                min = height;
            rows.push_back(height);
        }
        for (int j = 0; j < zl; j++) {
            rows[j] -= min - 1;
            // std::cout << rows[j] << std::endl;
        }
        heightmap.push_back(rows);
    }
}

int ***generateWorld() {
    int ***blockdata = (int ***)malloc(xl * sizeof(int **));
    for (int i = 0; i < xl; i++) {
        blockdata[i] = (int **)malloc(yl * sizeof(int *));
        for (int j = 0; j < yl; j++) {
            blockdata[i][j] = (int *)calloc(zl, sizeof(int));
        }
    }
    generateheightmap();
    for (int i = 0; i < xl; i++) {
        for (int k = 0; k < zl; k++) {
            float height = heightmap[i][k];
            for (int j = 0; j < (int)height; j++) {
                if (height < yl){
                    blockdata[i][j][k] = 1;
                }
            }
        }
    }
    return blockdata;
}

void freedata(int ***mat) {
    for (int i = 0; i < xl; i++) {
        for (int j = 0; j < yl; j++) {
            free(mat[i][j]);
        }
        free(mat[i]);
    }
    free(mat);
}
