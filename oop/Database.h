#pragma once
#include "Chunk.h"
#include <iostream>
#include <fstream>
#include <string>
class Database
{
private:
public:
	static void placeChunk(Chunk c);
	static Chunk getChunk(int x, int y, int z);
};

