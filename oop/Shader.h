#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//#include <GL/glew.h>; // Подключаем glew для того, чтобы получить все необходимые заголовочные файлы OpenGL
#include <glad/glad.h>

class Shader
{ 
private:
    //GLuint program;
public:
    GLuint program;
    // Идентификатор программы
    // Конструктор считывает и собирает шейдер
    Shader();
    Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
    // Использование программы
    void use();
    GLuint getUniformLocation(const GLchar* name);
};