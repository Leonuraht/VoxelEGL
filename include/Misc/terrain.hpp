#ifndef TERRAIN_H
#define TERRAIN_H

#include <glad/glad.h>
#include <glm/glm.hpp>

int ***generateWorld();
std::vector<float> generatefaces(int ***worlddata);
void freedata(int ***mat);

struct chunk_block {
    uint8_t posx : 4, posy : 4;
    uint8_t blocktype;
};

class Chunk {
  public:
    int x, y, z;
    Chunk(int x, int y, int z);
    struct chunk_block *blocks;
    void draw();
};

#endif
