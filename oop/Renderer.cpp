#include "Renderer.h"

Renderer::Renderer(Shader ssh, Shader gs, Texture ta, Texture na, Texture ssp, int w, int h): screenShader(ssh), gameShader(gs), textureAtlas(ta), normaleAtlas(na), skySphere(ssp), width(w), height(h)
{
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glGenTextures(1, &bufferTexture);
    glBindTexture(GL_TEXTURE_2D, bufferTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GL_NEAREST));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GL_NEAREST));
    //int width = 1920, height = 1080;
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, bufferTexture, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    glBindTexture(GL_TEXTURE_2D, bufferTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::start()
{
    meshRenderer = new MeshRenderer();
}

void Renderer::render()
{
    renderGame();
    postRenderGame();
}

void Renderer::renderGui()
{
}

void Renderer::renderGame()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glEnable(GL_DEPTH_TEST);
    //glClearColor(0.42f, 0.42f, 0.42f, 0.0f); cool sky color
    glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, textureAtlas.getTexture());

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, normaleAtlas.getTexture());
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, skySphere.getTexture());
    //gameShader.use();
}

void Renderer::postRenderGame()
{
    //meshRenderer->render();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glDisable(GL_DEPTH_TEST);
    glClearColor(0.4f, 0.7f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    //glUniform1i(screenTexLoc, 0);
    glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, texture2.getTexture());
    glBindTexture(GL_TEXTURE_2D, bufferTexture);
}

Shader* Renderer::getGameShader()
{
    return &gameShader;
}

Shader* Renderer::getScreenShader()
{
    return &screenShader;
}

MeshRenderer* Renderer::getMeshRenderer()
{
    return meshRenderer;
}

void Renderer::setScreenSize(glm::ivec2 s)
{
    width = s.x, height = s.y;
}
