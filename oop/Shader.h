#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#include <GL/glew.h>; // ���������� glew ��� ����, ����� �������� ��� ����������� ������������ ����� OpenGL
#include <glad/glad.h>

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
    void setMat4(glm::mat4 m, std::string name);
    void setVec3(glm::vec3 v, std::string name);
    void setIvec3(glm::ivec3 v, std::string name);
    void setInt(GLuint i, std::string name);
    void setFloat(GLuint i, std::string name);
};