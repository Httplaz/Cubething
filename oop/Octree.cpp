#include "Octree.h"

Octree::Octree(int extent) : extent(extent)
{
	extents = new GLubyte[extent];
	int c = 1;
	maxSide = 1;
	for (int i = 0; i < extent; i++)
	{
		extents[i] = c;
		c = (c * 8) + 1;
		maxSide *= 2;
	}
	map = new GLubyte[extents[extent - 1]];
	for (int i = 0; i < extents[extent - 1]; i++)
		map[i] = 0;
}

Octree::~Octree()
{
	if (map != nullptr)
		delete[] map;
	if (extents != nullptr)
		delete[] extents;
}

void Octree::getPtr(glm::ivec3 pos)
{
}

void Octree::placeCube(glm::ivec3 pos, bool cube)
{
	int c = maxSide;
	int ptr = 0;
	std::stack<int> ptrs;
	for (int i = extent - 1; i >= 0; i--)
	{
		c /= 2;
		int segment = pos.x / c * 1 + pos.y / c * 2 + pos.z / c * 4;
		pos %= c;
		ptr += extents[i] * segment;
		ptrs.push(ptr);
	}

	int cn = cube - map[ptr];

	for (int i = 0; i < extent; i++)
	{
		map[ptrs.top()] += cn;
		ptrs.pop();
	}
}
