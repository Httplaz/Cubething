// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "Database.h"
std::string Database::world = "";

void Database::placeChunk(Chunk* ch)
{
	glm::ivec3 s = ch->getSize();
	glm::ivec3 p = ch->getPosition();
	std::string filename = ".\\chunks.\\" + world + std::to_string(p.x) + "." + std::to_string(p.y) + "." + std::to_string(p.z);
	std::ofstream file (filename + ".txt", std::ios::out | std::ios::binary);

	// Write to the file
	//file << std::to_string(p.x);
	//const char* niggers = "niggers";
	//file.write(niggers, sizeof(char)*7);

	file.write((char*)&(*(ch->getMap())), sizeof(GLubyte)*s.x*s.y*s.z*3);

	// Close the file
	file.close();
}

Chunk* Database::getChunk(int x, int y, int z)
{
	std::string filename = ".\\chunks.\\" + world + std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z);
	std::ifstream file(filename + ".txt", std::ios::out | std::ios::binary);
	if(!file.is_open())
	{ 
		//std::cerr << " failed to open file" << std::endl;
		return nullptr;
	}

	Chunk* chunk = new Chunk(x,z);
	glm::ivec3 s = chunk->getSize();
	file.read((char*)chunk->getMap(), sizeof(GLubyte)*s.x*s.y*s.z*2);
	return chunk;
}

void Database::setWorld(std::string name)
{
	world = name;
}
