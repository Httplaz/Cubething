#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "lodepng.h"

//#include <GL/glew.h>; // Подключаем glew для того, чтобы получить все необходимые заголовочные файлы OpenGL
#include <glad/glad.h>

class Texture
{
private:
    GLuint texture;
    GLuint width, height;
public:
    Texture();
    Texture(const GLchar* path, GLuint width, GLuint height, bool linear);
    GLuint getTexture();
};