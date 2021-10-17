#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "lodepng.h"

#include <GL/glew.h>; // ���������� glew ��� ����, ����� �������� ��� ����������� ������������ ����� OpenGL

class Texture
{
private:
    GLuint texture;
    GLuint width, height;
public:
    Texture();
    Texture(const GLchar* path, GLuint width, GLuint height);
    GLuint getTexture();
};