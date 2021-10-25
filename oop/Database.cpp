// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "Database.h"

void Database::placeChunk(Chunk c)
{
	glm::ivec3 s = c.getSize();
	glm::ivec3 p = c.getPosition();
	std::string filename = ".\\chunks.\\"+std::to_string(p.x) + "." + std::to_string(p.y) + "." + std::to_string(p.z);
	std::ofstream file (filename + ".txt", std::ios::out | std::ios::binary);

	// Write to the file
	//file << std::to_string(p.x);
	//const char* niggers = "niggers";
	//file.write(niggers, sizeof(char)*7);

	file.write((char*)&(*(c.getMap())), sizeof(GLubyte)*s.x*s.y*s.z*3);

	// Close the file
	file.close();
}

Chunk Database::getChunk(int x, int y, int z)
{
	std::string filename = ".\\chunks.\\" + std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z);
	std::ifstream file(filename + ".txt", std::ios::out | std::ios::binary);
	if(!file.is_open())
	{ 
		return Chunk();
		std::cerr << " failed to open file" << std::endl;
	}

	Chunk chunk(x,z);
	glm::ivec3 s = chunk.getSize();
	file.read((char*)chunk.getMap(), sizeof(GLubyte)*s.x*s.y*s.z*3);
	return chunk;
	//return nullptr;

}
