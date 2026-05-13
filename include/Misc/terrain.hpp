#ifndef TERRAIN_H
#define TERRAIN_H

#include <glad/glad.h>
#include <glm/glm.hpp>

class Chunk {
  public:
    int x, z;
    int8_t heightmap[324];
    Chunk(int x, int z);
    struct chunk_block *blocks;
    void draw();
};


int ***generateWorld(Chunk& chunk,int threadch);
std::vector<float> generatefaces(int ***worlddata,Chunk& chunk,int threadch);
void freedata(int ***mat);

struct chunk_block {
    uint8_t posx : 4, posy : 4;
    uint8_t blocktype;
};

#endif
