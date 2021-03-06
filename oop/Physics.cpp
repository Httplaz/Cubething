// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "Physics.h"
#define gf 2

glm::ivec3 Physics::raycastCube(glm::vec3 pos, glm::vec3 dir, GLubyte* map, glm::ivec3 size)
{
    while (pos.x >= 0 && pos.y >= 0-200 && pos.z >= 0 && pos.x < size.x && pos.y < size.y+200 && pos.z < size.z)
    {
        vec3 sdir = sign(dir);
        vec3 dist = (vec3(1., 1., 1.) - fract(pos)) * (sdir + vec3(1.)) / 2.f + fract(pos) * (sdir - vec3(1.)) / -2.f + 1e-4f;
        vec3 prior = dist / abs(dir);
        float m = min(prior.x, min(prior.y, prior.z));
        pos += dir * m;
        vec3 cube = floor(pos);
        ivec3 c = getCube(cube, map, size);
        if (c.x > 0)
            return cube;
    }
    return glm::ivec3(999);
}

glm::vec3 Physics::raycastCubeRaw(glm::vec3 pos, glm::vec3 dir, GLubyte* map, glm::ivec3 size)
{
    while (pos.x >= 0 && pos.y >= 0 - 100 && pos.z >= 0 && pos.x < size.x && pos.y < size.y + 200 && pos.z < size.z)
    {
        vec3 sdir = sign(dir);
        vec3 dist = (vec3(1., 1., 1.) - fract(pos)) * (sdir + vec3(1.)) / 2.f + fract(pos) * (sdir - vec3(1.)) / -2.f + 1e-4f;
        vec3 prior = dist / abs(dir);
        float m = min(prior.x, min(prior.y, prior.z));
        pos += dir * m;
        vec3 cube = floor(pos);
        ivec3 c = getCube(cube, map, size);
        if (c.x > 0)
            return pos;
    }
    return glm::vec3(999.);
}

glm::vec3 Physics::collideEntity(glm::vec3 pos, glm::vec3 delta, glm::mat4 rotation, GLubyte* map, glm::ivec3 size)
{
    glm::vec3 npos = pos + delta;
    glm::vec3 masks[8] = {glm::vec3(1.,1.,1.), glm::vec3(1.,1.,0.), glm::vec3(1.,0.,1.), glm::vec3(0.,1.,1.), glm::vec3(1.,0.,0.), glm::vec3(0.,1.,0.), glm::vec3(0.,0.,1.), glm::vec3(0.,0.,0.)};
    std::vector<int> masksVector;
    for (int i = 0; i < 8; i++)
    {
        bool available = true;
        for (int x = floor(npos.x - 0.5f); x < ceil(npos.x + 0.5f); x++)
            for (int y = floor(npos.y - 1.4f - 0.5f); y < ceil(npos.y + 0.5f); y++)
                for (int z = floor(npos.z - 0.5f); z < ceil(npos.z + 0.5f); z++)
                    if (checkIntersection(glm::ivec3(x, y, z), pos + delta * masks[i]) && getCube(glm::ivec3(x,y,z),map,size).x!=0)
                    {
                        available = false;
                        break;
                    }
        if (available)
            masksVector.push_back(i), i = 8;
    }
    if (masksVector.size() > 0)
        return(pos + delta * masks[masksVector[0]]);
    else
        return (pos + glm::vec3(0, 1, 0));


    return glm::vec3(npos);
}

glm::ivec3 Physics::getCube(glm::ivec3 pos, GLubyte* map, glm::ivec3 mapSize)
{
    if (pos.x >= 0 && pos.x < mapSize.x && pos.y >= 0 && pos.y < mapSize.y && pos.z >= 0 && pos.z < mapSize.z)
    {
        int i = mapSize.y * mapSize.x * pos.z + mapSize.x * pos.y + pos.x;
        return glm::ivec3(map[i * gf], map[i * gf + 1], map[i * gf + 2]);
    }
    else
        return glm::ivec3(0, 0, 0);
}

glm::ivec3 Physics::getCubeNormale(glm::vec3 pos, glm::vec3 cube)
{
    vec3 center = cube + vec3(0.5);
    vec3 dist = pos - center;
    vec3 adist = abs(dist);
    vec3 sdist = sign(dist);
    vec3 fpos = fract(pos);
    if (adist.z > adist.y)
    {
        if (adist.z > adist.x)
            return vec3(fpos.x, fpos.y, 3 + 3 * (sign(sdist.z) - 1.) / 2.);
        else
            return vec3(fpos.z, fpos.y, 4 + 3 * (sign(sdist.x) - 1.) / 2.);
    }
    else
    {
        if (adist.y > adist.x)
            return vec3(fpos.z, fpos.x, 5 + 3 * (sign(sdist.y) - 1.) / 2.);
        else
            return vec3(fpos.z, fpos.y, 4 + 3 * (sign(sdist.x) - 1.) / 2.);
    }
}

glm::ivec3 Physics::cubeNormale(int n)
{
    ivec3 normales[6]{ vec3(0,0, -1), vec3(-1, 0, 0), vec3(0, -1, 0), vec3(0,0,1), vec3(1,0,0), vec3(0, 1, 0) };
    return normales[n];
}

bool Physics::checkIntersection(glm::ivec3 vpos, glm::vec3 pos)
{
    pos += glm::vec3(-0.4f, -1.4f, -0.4f);
    return !(pos.y > vpos.y+1 || pos.y + 1.4 < vpos.y || pos.x > vpos.x + 1 || pos.x + 0.8 < vpos.x || pos.z > vpos.z + 1 || pos.z + 0.8 < vpos.z);
}
