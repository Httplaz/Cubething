
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include "Shader.h"


Shader::Shader()
{}
Shader::Shader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
    // 1. retrieve the vertex/fragment source code from filePath
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    // ensure ifstream objects can throw exceptions:
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // open files
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        // read file's buffer contents into streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // close file handlers
        vShaderFile.close();
        fShaderFile.close();
        // convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // 2. Сборка шейдеров
    GLuint vertex, fragment;
    GLint success;
    GLchar infoLog[4096];

    // Вершинный шейдер
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // Если есть ошибки - вывести их
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 4096, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    };
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "VERTEX SHADER INFO\n" << infoLog << std::endl;

    // Аналогично для фрагментного шейдера
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // Если есть ошибки - вывести их
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertex, 4096, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    };

    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "FRAGMENT SHADER INFO\n" << infoLog << std::endl;

    // Шейдерная программа
    this->program = glCreateProgram();
    glAttachShader(this->program, vertex);
    glAttachShader(this->program, fragment);
    glLinkProgram(this->program);
    //Если есть ошибки - вывести их
    glGetProgramiv(this->program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(this->program, 4096, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glGetProgramInfoLog(this->program, 512, NULL, infoLog);
    std::cout << "SHADER PROGRAM INFO\n" << infoLog << std::endl;
    // Удаляем шейдеры, поскольку они уже в программу и нам больше не нужны.
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(this->program); 
}

GLuint Shader::getUniformLocation(const GLchar* name)
{
    return glGetUniformLocation(this->program, name);
}
