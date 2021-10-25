// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "WorldGenerator.h"

WorldGenerator::WorldGenerator()
{
}

WorldGenerator::WorldGenerator(int h) : height(h)
{
}

glm::ivec2 WorldGenerator::getCube(glm::ivec3 pos)
{
    float sinx = abs(sin(glm::radians(180. * pos.x / 16.)));
    //float sinx = 0;
    float sinz = abs(sin(glm::radians(180. * pos.z / 16.)));
    //float sinz = 1;

    //int h = (int)(height * (0.6 + sinx * sinz / 2.));
    int h = height / 2;// *(0.5f + abs(sin(pos.x / 400.f) * cos(pos.z / 900.f)));
    //return glm::vec2(0, 0);
    //if (this == nullptr)
        //h /= a[0];
    //if ((pos.z / 16) % 2 == 0 && (pos.x / 16) % 2 == 0)
        //return glm::ivec2(3, 0);
    //else
        //return glm::ivec2(1, 1);
    //return glm::vec2(0., 0.);
    if (pos.y == h-1)
        return glm::vec2(1., 1.);
    else if (pos.y < h)
        return glm::vec2(3., 0.);
    else return glm::vec2(0., 0.);
        //return glm::ivec2(3,0);
    //if (pos.z == 0)
        //return glm::ivec3(1, 1,0);
}

void WorldGenerator::fillMap(glm::vec3 pos, GLubyte* map, glm::ivec3 mapSize)
{
    //if (height != 4)
        //return;
    for (int x = 0; x < mapSize.x; x++)
        for (int y = 0; y < mapSize.y; y++)
            for (int z = 0; z < mapSize.z; z++)
            {
                glm::vec2 cube = getCube({ pos.x+x, pos.y+y, pos.z+z });
                int i = mapSize.y * mapSize.x * z + mapSize.x * y + x;
                map[i * 3] = cube.x;
                map[i * 3 + 1] =  cube.y;
                //map[i * 3 + 0] = 1;
                //map[i * 3 + 1] = 1;
            }
}
