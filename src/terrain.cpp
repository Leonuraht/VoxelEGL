#include "Misc/terrain.hpp"
#include <cstdlib>
#include <iostream>

int xl = 100,yl = 10,zl = 100;


float frontface[] = {
    // --- Front Face (Z = +0.5) ---
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Bottom-Left
    0.5f,  -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Bottom-Right
    0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Top-Right
    0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Top-Right
    -0.5f, 0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // Top-Left
    -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, // Bottom-Left
};
float backface[] = {
    // --- Back Face (Z = -0.5) ---
    0.5f,  -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Bottom-Left
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Bottom-Right
    -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, -1.0f, // Top-Right
    -0.5f, 0.5f,  -0.5f, 0.0f, 0.0f, -1.0f, // Top-Right
    0.5f,  0.5f,  -0.5f, 0.0f, 0.0f, -1.0f, // Top-Left
    0.5f,  -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, // Bottom-Left
};
float leftface[] = {
    // --- Left Face (X = -0.5) ---
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // Bottom-Left
    -0.5f, -0.5f, 0.5f,  -1.0f, 0.0f, 0.0f, // Bottom-Right
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f, 0.0f, // Top-Right
    -0.5f, 0.5f,  0.5f,  -1.0f, 0.0f, 0.0f, // Top-Right
    -0.5f, 0.5f,  -0.5f, -1.0f, 0.0f, 0.0f, // Top-Left
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, // Bottom-Left
};
float rightface[] = {
    // --- Right Face (X = +0.5) ---
    0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, // Bottom-Left
    0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // Bottom-Right
    0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, // Top-Right
    0.5f, 0.5f,  -0.5f, 1.0f, 0.0f, 0.0f, // Top-Right
    0.5f, 0.5f,  0.5f,  1.0f, 0.0f, 0.0f, // Top-Left
    0.5f, -0.5f, 0.5f,  1.0f, 0.0f, 0.0f, // Bottom-Left
};
float topface[] = {
    // --- Top Face (Y = +0.5) ---
    -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f, // Bottom-Left
    0.5f,  0.5f, 0.5f,  0.0f, 1.0f, 0.0f, // Bottom-Right
    0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top-Right
    0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top-Right
    -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // Top-Left
    -0.5f, 0.5f, 0.5f,  0.0f, 1.0f, 0.0f, // Bottom-Left
};
float bottomface[] = {
    // --- Bottom Face (Y = -0.5) ---
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // Bottom-Left
    0.5f,  -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, // Bottom-Right
    0.5f,  -0.5f, 0.5f,  0.0f, -1.0f, 0.0f, // Top-Right
    0.5f,  -0.5f, 0.5f,  0.0f, -1.0f, 0.0f, // Top-Right
    -0.5f, -0.5f, 0.5f,  0.0f, -1.0f, 0.0f, // Top-Left
    -0.5f, -0.5f, -0.5,  0.0f, -1.0f, 0.0f  // Bottom-Left
};


std::vector<float> generatefaces(int*** data) {
    std::vector<float> mesh;
    mesh.reserve(500000);
    for (int i = 0; i < xl; i++) {
        for (int j = 0; j < yl; j++) {
            for (int k = 0; k < zl; k++) {
                if (data[i][j][k] <= 0)
                    continue;
                float xoff = i - xl/2.f, yoff = j - yl/2.f, zoff = k - yl/2.f;

                if (i == xl-1 || data[i + 1][j][k] == 0) {
                    for (int v = 0; v < 36; v += 6) {
                        mesh.push_back(rightface[v] + xoff);
                        mesh.push_back(rightface[v + 1] + yoff);
                        mesh.push_back(rightface[v + 2] + zoff);
                        mesh.push_back(rightface[v + 3]);
                        mesh.push_back(rightface[v + 4]);
                        mesh.push_back(rightface[v + 5]);
                    }
                }
                if (i == 0 || data[i - 1][j][k] == 0) {
                    for (int v = 0; v < 36; v += 6) {
                        mesh.push_back(leftface[v] + xoff);
                        mesh.push_back(leftface[v + 1] + yoff);
                        mesh.push_back(leftface[v + 2] + zoff);
                        mesh.push_back(leftface[v + 3]);
                        mesh.push_back(leftface[v + 4]);
                        mesh.push_back(leftface[v + 5]);
                    }
                }

                if (k == zl-1 || data[i][j][k + 1] == 0) {
                    for (int v = 0; v < 36; v += 6) {
                        mesh.push_back(frontface[v] + xoff);
                        mesh.push_back(frontface[v + 1] + yoff);
                        mesh.push_back(frontface[v + 2] + zoff);
                        mesh.push_back(frontface[v + 3]);
                        mesh.push_back(frontface[v + 4]);
                        mesh.push_back(frontface[v + 5]);
                    }
                }

                if (k == 0 || data[i][j][k - 1] == 0) {
                    for (int v = 0; v < 36; v += 6) {
                        mesh.push_back(backface[v] + xoff);
                        mesh.push_back(backface[v + 1] + yoff);
                        mesh.push_back(backface[v + 2] + zoff);
                        mesh.push_back(backface[v + 3]);
                        mesh.push_back(backface[v + 4]);
                        mesh.push_back(backface[v + 5]);
                    }
                }

                if (j == yl-1 || data[i][j + 1][k] == 0) {
                    for (int v = 0; v < 36; v += 6) {
                        mesh.push_back(topface[v] + xoff);
                        mesh.push_back(topface[v + 1] + yoff);
                        mesh.push_back(topface[v + 2] + zoff);
                        mesh.push_back(topface[v + 3]);
                        mesh.push_back(topface[v + 4]);
                        mesh.push_back(topface[v + 5]);
                    }
                }

                if (j == 0 || data[i][j - 1][k] == 0) {
                    for (int v = 0; v < 36; v += 6) {
                        mesh.push_back(bottomface[v] + xoff);
                        mesh.push_back(bottomface[v + 1] + yoff);
                        mesh.push_back(bottomface[v + 2] + zoff);
                        mesh.push_back(bottomface[v + 3]);
                        mesh.push_back(bottomface[v + 4]);
                        mesh.push_back(bottomface[v + 5]);
                    }
                }
            }
        }
    }
    freedata(data);
    return mesh;
}

int*** generateWorld() {
    int*** blockdata = (int***) malloc(xl * sizeof(int**));
    for(int i = 0;i < xl;i++){
        blockdata[i] = (int**) malloc(yl * sizeof(int*));
        for(int j = 0;j < yl;j++){
            blockdata[i][j] = (int*) malloc(zl * sizeof(int));
        }
    }

    for (int i = 0; i < xl; i++) {
        for (int j = 0; j < yl; j++) {
            for (int k = 0; k < zl; k++) {
                if (j > yl/2.f) {
                    blockdata[i][j][k] = 0;
                } else {
                    float oo = (float)random()/INT_MAX;
                    if(oo > 0.5) blockdata[i][j][k] = 1;
                    else blockdata[i][j][k] = 0;
                }
            }
        }
    }
    return blockdata;
}

void freedata(int*** mat){
    for(int i = 0;i < xl;i++){
        for(int j = 0;j < yl;j++){
            free(mat[i][j]);
        }
        free(mat[i]);
    }
    free(mat);
}
