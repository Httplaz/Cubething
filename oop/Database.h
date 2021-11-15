#pragma once
#include "Chunk.h"
#include <iostream>
#include <fstream>
#include <string>
#include <glm/glm.hpp>
class Database
{
private:
	static std::string world;
public:
	static void placeChunk(Chunk* ch);
	static Chunk* getChunk(int x, int y, int z);
	static void fillChunk(int x, int y, int z, Chunk* ch);
	static void setWorld(std::string name);
};

