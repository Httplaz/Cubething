#include "Texture.h"

Texture::Texture()
{
}

Texture::Texture(const GLchar* path, GLuint width, GLuint height)
{
    std::vector<unsigned char> image;
    this->width = width;
    this->height = height;
    unsigned error = lodepng::decode(image, width, height, path);

    // If there's an error, display it.
    if (error != 0) 
    {
        std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
    }

    //GLubyte* map = new GLubyte[3 * height * width];
    //for (int i = 0; i < height * width; i++)
        //map[i * 3] = 126, map[i * 3 + 1] = 251, map[i * 3 + 2] = 251;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image.front());
    //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, map);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint Texture::getTexture()
{
    return texture;
}
