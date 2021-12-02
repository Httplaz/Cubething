// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "World.h"
#define gf 2

void World::streamSwapMapChunks(int i0, int j0, int i1, int j1)
{
	//int k0 = j0 * load + i0;
	//int k1 = j1 * load + i1;
	int wh = width * height;
	glm::ivec3 chunkSize = Chunk::getSize();
	for (int x = 0; x < chunkSize.x; x++)
		for (int y = 0; y < chunkSize.y; y++)
			for (int z = 0; z < chunkSize.z; z++)
			{
				int k0 = (z + j0 * chunkSize.z) * wh + y * width + (x + i0 * chunkSize.x);
				int k1 = (z + j1 * chunkSize.z) * wh + y * width + (x + i1 * chunkSize.x);
				//std::swap(map[k0 * gf], map[k1 * gf]);
				//std::swap(map[k0 * gf + 1], map[k1 * gf + 1]);
			}

	int widthl = (width), heightl = (height);
	glm::ivec3 lcs = chunkSize;
	for (int i = 0; i < mipmapLevel; i++)
	{
		lcs /= 2;
		widthl /= 2;
		heightl /= 2;
		for (int x = 0; x < lcs.x; x++)
			for (int y = 0; y < lcs.y; y++)
				for (int z = 0; z < lcs.z; z++)
				{
					int xl0 = i0 * lcs.x + x, zl0 = j0 * lcs.z + z, xl1 = i1 * lcs.x + x, zl1 = j1 * lcs.z + z, yl = y;
					int kl0 = heightl * widthl * zl0 + widthl * yl + xl0;
					int kl1 = heightl * widthl * zl1 + widthl * yl + xl1;
					std::swap(mipmap[i][kl0 * gf], mipmap[i][kl1 * gf]);
					//std::swap(mipmap[i][kl0 * gf+1], mipmap[i][kl1 * gf+1]);
				}
	}

}

World::World() { map = nullptr; }

World::World(int ld, WorldGenerator* worldGen, MeshRenderer* renderer) : width(ld* Chunk::getSize().x), height(Chunk::getSize().x), depth(ld* Chunk::getSize().z), worldGenerator(worldGen), load(ld), meshRenderer(renderer)
{
	glm::ivec3 cs = Chunk::getSize();
	width = load * cs.x;
	height = cs.y;
	depth = load * cs.z;
	map = new GLubyte[gf * width * height * depth];
	for (int i = 0; i < gf * width * height * depth; i++)
		map[i] = 3;


	loadedChunks = new Chunk * [load * load];
	for (int i = 0; i < load; i++)
		for (int j = 0; j < load; j++)
			loadedChunks[i * load + j] = getChunkAbs(i + chunkOffset.x, 0, j + chunkOffset.z);		
	mipmap = new GLubyte * [6];

	for (int i = 0, sc = 8; i < mipmapLevel; i++, sc *= 8)
	{
		mipmap[i] = new GLubyte[gf * width * height * depth / sc];
		for (int j = 0; j < gf * width * height * depth / sc; j++)
			mipmap[i][j] = 0;
	}
	glm::ivec3 chunkSize = Chunk::getSize();
	mipcdx = new int[mipmapLevel];
	mipcdz = new int[chunkSize.x * mipmapLevel];
	mipcdy = new int[chunkSize.z * chunkSize.x * mipmapLevel];
	loadNeeded = new bool[load * load];
	for (int i = 0; i < load * load; i++)
		loadNeeded[i] = true;
	generationNeeded = new bool[load * load];
	for (int i = 0; i < load * load; i++)
		generationNeeded[i] = true;
	toBuild = new bool[load*load];
	for (int i = 0; i < load * load; i++)
		toBuild[i] = false;
	toUpdate = new bool[load * load];
	for (int i = 0; i < load * load; i++)
		toUpdate[i] = false;
	loadedChunksChanged = new bool[load * load];
	for (int i = 0; i < load * load; i++)
		loadedChunksChanged[i] = false;
	loadedChunksNeeded = new bool[load * load];
	for (int i = 0; i < load * load; i++)
		loadedChunksNeeded[i] = false;


	chunkSamplerOffset = new int[load*load*2];
	for (int i = 0; i < load; i++)
		for (int j=0; j<load; j++)
			chunkSamplerOffset[(j*load+i)*2] = i, chunkSamplerOffset[(j * load + i) * 2+1] = j;
	
}


World& World::operator=(const World& w)
{
	std::cout << "THIS IS WRONG\n\n\n\n\n\n\n";
	width = w.width;
	height = w.height;
	depth = w.depth;
	map = w.map;
	loadedChunksChanged = w.loadedChunksChanged;
	loadedChunks = w.loadedChunks;
	worldGenerator = w.worldGenerator;
	mipmap = w.mipmap;
	return *this;
}

World::World(const World& other)
{
	std::cout << "THIS IS TRUE YOU FUCKER\n";
}


World::~World()
{
	delete[] map;
	delete[] mipcdx;
	delete[] mipcdz;
	delete[] mipcdy;
	delete[] loadNeeded;
}
void World::updateMapSampler()
{

	int* pp = new int[load * load * 2];
	for (int i = 0; i < load * load * 2; i++)
		pp[i] = 3;

	glGenTextures(1, &chunkOffsetSampler);
	glBindTexture(GL_TEXTURE_2D, chunkOffsetSampler);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_3D, GL_GENERATE_MIPMAP, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG8UI, load, load, 0, GL_RG_INTEGER, GL_UNSIGNED_BYTE, pp);
	delete [] pp;

	updateMipmap();
	//std::cout << "WHAT\n\n\n\n\n";
	if (!samplerLoaded)
	{
		glGenTextures(1, &mapSampler);
		glBindTexture(GL_TEXTURE_3D, mapSampler);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_3D, GL_GENERATE_MIPMAP, GL_TRUE);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RG8UI, width, height, depth, 0, GL_RG_INTEGER, GL_UNSIGNED_BYTE, NULL);
		//glTexImage3D(GL_TEXTURE_3D, 0, GL_RG8UI, width*2*4, height/4, depth/3, 0, GL_RG_INTEGER, GL_UNSIGNED_BYTE, mapB);
		//glTexImage3D(GL_TEXTURE_3D, 1, GL_RG8UI, load, load, 1, 0, GL_RG_INTEGER, GL_UNSIGNED_BYTE, map);
		for (int i = 0, sc = 2; i < mipmapLevel; i++, sc *= 2)
		{
			glTexImage3D(GL_TEXTURE_3D, i + 1, GL_RG8UI, width / sc, height / sc, depth / sc, 0, GL_RG_INTEGER, GL_UNSIGNED_BYTE, mipmap[i]);

		}
		//glTexImage3D(GL_TEXTURE_3D, i + 1, GL_RG8UI, width / sc, height / sc, depth / sc, 0, GL_RG_INTEGER, GL_UNSIGNED_BYTE, mipmap[i]);
		samplerLoaded++;
	}
	else
	{
		for (int i = 0, sc = 2; i < mipmapLevel; i++, sc *= 2)
		{
			glTexSubImage3D(GL_TEXTURE_3D, i + 1, 0, 0, 0, width / sc, height / sc, depth / sc, GL_RG_INTEGER, GL_UNSIGNED_BYTE, mipmap[i]);
		}
	}
	for (int i = 0; i < load; i++)
		for (int j = 0; j < load; j++)
			toUpdate[i * load + j] = true;
}

void World::updateChunkSampler(int i, int j)
{
	glm::ivec3 chunkSize = Chunk::getSize();
	glBindTexture(GL_TEXTURE_3D, mapSampler);
	glTexSubImage3D(GL_TEXTURE_3D, 0, i * chunkSize.x, 0, j * chunkSize.z, chunkSize.x, height, chunkSize.z, GL_RG_INTEGER, GL_UNSIGNED_BYTE, loadedChunks[i * load + j]->getMap());
}

void World::updateMapMesh()
{
	glm::ivec3 cs = Chunk::getSize();
	for (int i = 0; i < load; i++)
		for (int j = 0; j < load; j++)
			if (loadNeeded[i * load + j])
				if (!samplerLoaded)
					loadedChunks[i * load + j]->prepareRender(map, glm::ivec3(cs.x * i, 0, cs.z * j), glm::ivec3(cs.x * (i + 1), cs.y, cs.z * (j + 1)), width, height, depth);
				else
					toBuild[i * load + j] = true;
					
	//std::cout << samplerLoaded << " " << "NEW MESH CREATED\n";
}

void World::debug()
{
	for (int i = 0; i < load; i++)
		for (int j = 0; j < load; j++)
			loadedChunks[i * load + j]->render();
}

void World::reloadImage()
{
	std::cout << "reloading";
	for (int i=0; i<load; i++)
		for (int j = 0; j < load; j++)
		{
			toBuild[i * load + j] = true;
			loadNeeded[i * load + j] = true;
		}
	samplerLoaded = false;
	//updateMipmap();
	updateMapSampler();
	glm::ivec3 cs = Chunk::getSize();
	//for (int i = 0; i < load; i++)
		//for (int j = 0; j < load; j++)
			//updateMapSamplerS(glm::ivec3(i * cs.x, 0, j * cs.z));
	updateMapMesh();
	
}

void World::updateMapSamplerS(glm::ivec3 pos)
{
	glm::ivec3 cs = Chunk::getSize();
	int k = height * width * pos.z + width * pos.y + pos.x;

	glm::ivec3 cube = getCube(pos);
	map[k * gf] = cube.x;
	map[k * gf + 1] = cube.y;
	if (cube.x > 0)
		for (int i = 0, sc = 2; i < mipmapLevel; i++)
		{
			int x1 = pos.x / sc, y1 = pos.y / sc, z1 = pos.z / sc, width1 = width / sc, height1 = height / sc, depth1 = depth / sc;
			int k1 = height1 * width1 * z1 + width1 * y1 + x1;
			mipmap[i][k1 * gf] = 1;
			mipmap[i][k1 * gf + 1] = 0;
			sc *= 2;
		}

	GLuint* m1 = new GLuint[gf];
	for (int i = 0; i < gf; i++)
		m1[i] = 3;

	//TODO

	glBindTexture(GL_TEXTURE_3D, mapSampler);
	glTexSubImage3D(GL_TEXTURE_3D, 0, pos.x, pos.y, pos.z, 1, 1, 1, GL_RGB_INTEGER, GL_UNSIGNED_BYTE, m1);
	//glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, width, height, depth, GL_RG_INTEGER, GL_UNSIGNED_BYTE, map);
	for (int i = 0, sc = 2; i < mipmapLevel; i++, sc *= 2)
	{
		glTexSubImage3D(GL_TEXTURE_3D, i + 1, 0, 0, 0, width / sc, height / sc, depth / sc, GL_RG_INTEGER, GL_UNSIGNED_BYTE, mipmap[i]);
	}

	int i = pos.x / cs.x, j = pos.z / cs.z;
	//updateMapMesh();

}

void World::updateMapMeshS(glm::ivec3 pos)
{
	glm::ivec3 cs = Chunk::getSize();
	int i = pos.x / cs.x, j = pos.z / cs.z;
	toBuild[i * load + j] = true;
	//loadedChunks[i * load + j]->prepareRender(map, glm::ivec3(cs.x * i, 0, cs.z * j), glm::ivec3(cs.x * (i + 1), cs.y, cs.z * (j + 1)), width, height, depth);//, loadedChunks[i * load + j]->prepareRender(map, glm::ivec3(cs.x * i, 0, cs.z * j), glm::ivec3(cs.x * (i + 1), cs.y, cs.z * (j + 1)), width, height, depth);
}

GLuint World::getMapSampler()
{
	return mapSampler;
}

int* World::getChunkSamplerOffset()
{
	return chunkSamplerOffset;
}

void World::updateMipmap()
{
	glm::ivec3 cs = Chunk::getSize();

	glm::ivec3 chunkSize = Chunk::getSize();

	int wh = width * height;
	for (int y = 0; y < height; y++)
	{
		for (int i = 0; i < load; i++)
			for (int j = 0; j < load; j++)
			{
				if (loadNeeded[i * load + j])
				{
					std::fill(mipcdy, mipcdy + mipmapLevel * chunkSize.x * chunkSize.z, 0);
					GLubyte* m = loadedChunks[load * i + j]->getMap();
					std::fill(mipcdz, mipcdz + chunkSize.x * mipmapLevel, 0);
					//int ck = 0;
					for (int z = 0; z < chunkSize.z; z++)
					{

						std::fill(mipcdx, mipcdx + mipmapLevel, 0);
						for (int x = 0; x < chunkSize.x; x++)
						{
							int az = (z + j * chunkSize.z);
							int ax = (x + i * chunkSize.x);
							int ck = z * chunkSize.x * chunkSize.y + y * chunkSize.x + x;
							int k = az * wh + y * width + ax;
							map[k * gf] = m[ck * 2];
							map[k * gf + 1] = m[ck * 2 + 1];


							if (map[k * gf] > 0 && mipcdx[0] - x <= 0 && mipcdz[x * mipmapLevel + 0] - z * mipmapLevel - x <= 0 && mipcdy[x * chunkSize.x * mipmapLevel + z * mipmapLevel + 0] - y * chunkSize.x - z * mipmapLevel - x <= 0)
							{
								int widthl = (width), heightl = (height), xl = (ax), zl = (az), yl = (y);
								int sc = 2;
								for (int l = 0; l < mipmapLevel; l++)
								{
									if (mipcdx[l] - x > 0 && mipcdz[x * mipmapLevel + l] - z * mipmapLevel - x > 0 && mipcdy[x * chunkSize.x * mipmapLevel + z * mipmapLevel + l] - y * chunkSize.x - z * mipmapLevel - x > 0)
										break;
									widthl /= 2, heightl /= 2, xl /= 2, zl /= 2, yl /= 2;
									mipcdx[l] = sc - (ax - xl * sc) + x;
									mipcdz[x * mipmapLevel + l] = sc - (az - zl * sc) + z * mipmapLevel + x;
									mipcdy[x * chunkSize.x * mipmapLevel + z * mipmapLevel + l] = sc - (y - yl * sc) + y * chunkSize.x - z * mipmapLevel - x;
									int kl = heightl * widthl * zl + widthl * yl + xl;
									mipmap[l][kl * gf] = 1;
									mipmap[l][kl * gf + 1] = 0;
									sc *= 2;
								}
							}
						}
					}
				}
			}

	}
}

void World::update()
{
	int chunkPortion = load*load/16;
	glm::ivec3 cs = Chunk::getSize();
	for (int i = 0; i < load; i++)
		for (int j = 0; j < load; j++) 
		{
			if (loadedChunks[i * load + j]->getLoadingStatus() == 0)
				loadedChunks[i * load + j]->loadMesh();
			if (toUpdate[i * load + j] && chunkUpdatePeriod==0 && chunkPortion>0)
			{
				updateChunkSampler(i, j);
				chunkPortion--;
				if (chunkPortion == 0)
					chunkUpdatePeriod = maxChunkUpdatePeriod;

				GLubyte* m = loadedChunks[i * load + j]->getMap();

				for (int x = 0; x < cs.x; x++)
					for (int y = 0; y < cs.y; y++)
						for (int z = 0; z < cs.z; z++)
						{
							int az = (z + j * cs.z);
							int ax = (x + i * cs.x);
							int ck = z * cs.x * cs.y + y * cs.x + x;
							int k = az * width * height + y * width + ax;
							map[k * gf] = m[ck * 2];
							map[k * gf + 1] = m[ck * 2 + 1];
						}
				//updateMipmap();
				toUpdate[i * load + j] = false;
			}
		}
	chunkUpdatePeriod = std::max(0, chunkUpdatePeriod - 1);
	//if(chunkUpdatePeriod>0)
		//std::cout << chunkUpdatePeriod << "\n";
}

void World::offthread()
{
	glm::ivec3 cs = Chunk::getSize();
	while (true)
	{
		for(int a=0;a<load*load; a++)
			if (toBuild[a])
			{
				glm::ivec2 cO = chunkOffset;
				//int a = toBuild.top();
				int i = a / load;
				int j = a % load;
				toBuild[a] = false;
				std::vector<GLfloat> newVertices = loadedChunks[a]->buildMesh(map, glm::ivec3(cs.x * i, 0, cs.z * j), glm::ivec3(cs.x * (i + 1), cs.y, cs.z * (j + 1)), width, height, depth, loadedChunks[i*load+j]);
				//i += cO.x - chunkOffset.x;
				//j += cO.y - chunkOffset.y;
				loadedChunks[i*load+j]->setVertices(newVertices);
			}
	}
}

void World::startOffthread()
{
	std::thread thread(&World::offthread, this);
	thread.detach();
}

void World::render()
{
	meshRenderer->render();
}

void World::saveChunks()
{
	for (int i = 0; i < load * load; i++)
		if (loadedChunksChanged[i])
			Database::placeChunk(loadedChunks[i]);
}

void World::placeCube(glm::ivec3 pos, glm::ivec3 cube)
{
	glm::ivec3 chunk = pos / Chunk::getSize();
	glm::ivec3 relPos = glm::ivec3(pos.x % Chunk::getSize().x, pos.y % Chunk::getSize().y, pos.z % Chunk::getSize().z);
	loadedChunks[chunk.x * load + chunk.z]->placeCube(glm::ivec3(relPos.x, relPos.y, relPos.z), cube);
	loadedChunksChanged[chunk.x * load + chunk.z] = true;
	updateMapSamplerS(pos);
	updateMapMeshS(pos);
}

void World::setMeshRenderer(MeshRenderer* renderer)
{
	meshRenderer = renderer;
}

MeshRenderer* World::getMeshRenderer()
{
	return meshRenderer;
}

glm::ivec3 World::getCube(glm::ivec3 pos)
{
	if (pos.y >= Chunk::getSize().y || pos.y < 0)
		return glm::ivec3(999);
	glm::ivec3 chunk = pos / Chunk::getSize();
	glm::ivec3 relPos = glm::ivec3(pos.x % Chunk::getSize().x, pos.y, pos.z % Chunk::getSize().z);
	return loadedChunks[chunk.x * load + chunk.z]->getCube(relPos);
}

GLubyte* World::getMap()
{
	return map;
}

glm::ivec3 World::getSize()
{
	return glm::ivec3(width, height, depth);
}

glm::ivec3 World::getChunkOffset()
{
	return chunkOffset;
}

Chunk* World::getChunk(int x, int y, int z)
{
	int rx = x - chunkOffset.x;
	int rz = z - chunkOffset.z;
	if (rx >= 0 && rz >= 0 && rx < load && rz < load)
	{
		return loadedChunks[rx * load + rz];
	}
	else
		return getChunkAbs(x, y, z);
}

Chunk* World::getChunkAbs(int x, int y, int z)
{
	Chunk* ch = Database::getChunk(x, y, z);
	if (ch == nullptr)
		return new Chunk(x, z, worldGenerator);
	else
		return ch;
}

void World::setWorldGenerator(WorldGenerator* gen)
{
	worldGenerator = gen;
}

bool World::processesFinished()
{
	bool ans = true;
	for (int i=0; i<load*load; i++)
		if (toBuild[i] || toUpdate[i])
		{
			ans = false;
			break;
		}
	return ans;
}

glm::vec3 World::updateLoaded(glm::vec3 pos)
{
	glm::ivec3 chunkSize = Chunk::getSize();
	glm::vec3 playerPos = pos;
	glm::ivec2 deltaChunks(0);
	if (playerPos.x > chunkSize.x * (load / 2 + 1))
		playerPos.x -= chunkSize.x, deltaChunks.x++;
	if (playerPos.z > chunkSize.z * (load / 2 + 1))
		playerPos.z -= chunkSize.z, deltaChunks.y++;

	if (playerPos.x < chunkSize.x * (load - load / 2 - 1))
		playerPos.x += chunkSize.x, deltaChunks.x--;
	if (playerPos.z < chunkSize.z* (load - load / 2 - 1))
		playerPos.z += chunkSize.z, deltaChunks.y--;
	//deltaChunks = glm::ivec2(0);
	if (deltaChunks != glm::ivec2(0))
	{
		for (int i = 0; i < load * load; i++)
			loadNeeded[i] = true;
		std::fill(generationNeeded, generationNeeded + load * load, 1);
		int i0 = std::max(0, deltaChunks.x);
		int i1 = std::min(load, load + deltaChunks.x);
		int j0 = std::max(0, deltaChunks.y);
		int j1 = std::min(load, load + deltaChunks.y);
		bool sx = deltaChunks.x > 0;
		bool sy = deltaChunks.y > 0;
		saveChunks();
		//for (int i = 0; i < load * load; i++)
			//loadedChunksChanged[i] = false;
		toSwap.clear();
		for (int i = (sx ? i0 : i1 - 1); i != (sx ? i1 : i0 - 1); i += (sx ? 1 : -1))
			for (int j = (sy ? j0 : j1 - 1); j != (sy ? j1 : j0 - 1); j += (sy ? 1 : -1))
			{
				std::swap(loadedChunks[i * load + j], loadedChunks[(i - deltaChunks.x) * load + (j - deltaChunks.y)]);
				int k0 = j * load + i;
				int k1 = (j-deltaChunks.y) * load + (i+deltaChunks.x);
				streamSwapMapChunks(i, j, i - deltaChunks.x, j - deltaChunks.y);
				//toSwap.push_back(glm::ivec4(i, j, i - deltaChunks.x, j - deltaChunks.y));
				generationNeeded[(i - deltaChunks.x) * load + (j - deltaChunks.y)] = false;
				loadNeeded[(i - deltaChunks.x) * load + (j - deltaChunks.y)] = false;
			}

		Chunk** newLoadedChunks = new Chunk * [load * load];
		for (int i = 0; i < load; i++)
			for (int j = 0; j < load; j++)
			{
				if (generationNeeded[i * load + j] && !deb)
				{
					glm::ivec3 lcs = chunkSize;
					int widthl = width, heightl = height;
					for (int l = 0; l < mipmapLevel; l++)
					{
						lcs /= 2;
						widthl /= 2;
						heightl /= 2;
						for (int x = 0; x < lcs.x; x++)
							for (int y = 0; y < lcs.y; y++)
								for (int z = 0; z < lcs.z; z++)
								{
									int xl = i * lcs.x + x, zl = j * lcs.z + z, yl = y;
									int kl0 = heightl * widthl * zl + widthl * yl + xl;
									mipmap[l][kl0 * gf] = 0;
									//mipmap[l][kl0 * gf + 1] = 0;
								}
					}
					newLoadedChunks[i * load + j] = getChunk(chunkOffset.x + i + deltaChunks.x, chunkOffset.y, chunkOffset.z + j + deltaChunks.y), newLoadedChunks[i * load + j]->clearMesh();
					loadedChunks[i * load + j]->release();
					delete loadedChunks[i * load + j];
						//newLoadedChunks[i * load + j] = loadedChunks[i * load + j], loadedChunksNeeded[i * load + j] = true, newLoadedChunks[i * load + j]->moveMesh(glm::ivec2(deltaChunks.x * -chunkSize.x, deltaChunks.y * -chunkSize.z));// , loadedChunks[i * load + j]->release();
				}
				else
					newLoadedChunks[i * load + j] = loadedChunks[i * load + j], loadedChunksNeeded[i * load + j] = true, newLoadedChunks[i * load + j]->moveMesh(glm::ivec2(deltaChunks.x * -chunkSize.x, deltaChunks.y * -chunkSize.z));// , loadedChunks[i * load + j]->release();
				}
			delete[] loadedChunks;
			loadedChunks = newLoadedChunks;
		
		std::fill(loadedChunksChanged, loadedChunksChanged + load * load, 0);
		chunkOffset.x += deltaChunks.x;
		chunkOffset.z += deltaChunks.y;
		updateMapSampler();
		updateMapMesh();
	}
	return playerPos;

}
