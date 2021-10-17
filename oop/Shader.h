#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <GL/glew.h>; // ���������� glew ��� ����, ����� �������� ��� ����������� ������������ ����� OpenGL
#endif

class Shader
{ 
private:
    //GLuint program;
public:
    GLuint program;
    // ������������� ���������
    // ����������� ��������� � �������� ������
    Shader();
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // ������������� ���������
    void use();
    GLuint getUniformLocation(const GLchar* name);
};