#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>
#include <glad/glad.h>


int*** generateWorld();
std::vector<float> generatefaces(int*** worlddata);
void freedata(int*** mat);


#endif
