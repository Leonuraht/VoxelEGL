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
};

typedef struct {
    uint32_t data;
} Vertex;

uint8_t *generateWorld(Chunk &chunk, int threadch);
std::vector<Vertex> generatefaces(uint8_t *worlddata, Chunk &chunk,
                                 int threadch);
void freedata(uint8_t *mat);

#endif
