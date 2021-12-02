// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
#include <iostream>
#define PI 3.14512;
// GLEW
//#define GLEW_STATIC
//#include <GL/glew.h>
#define GLFW_INCLUDE_NONE

// GLFW
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Shader.h"
#include "Texture.h"
#include "World.h"
#include "WorldGenerator.h"
#include "Camera.h"
#include "Physics.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Player.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_stdlib.h"
#include "Settings.h"
#include "MeshRenderer.h"
#include "Renderer.h"
#include <vector>
#include <string>



void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void renderGame();
void renderGui();
void render();
void startGame();
void loadGame();
void startPoly();
void renderPoly();
void updatePolyMesh();
void createNewWorld();
void loadWorld();


GLuint WIDTH = 1920, HEIGHT = 1080;
GLFWwindow* window;
GLuint VBO, VAO, EBO;
Shader shader;
Texture texture;
Texture texture1;
Texture texture2;
World* world;
WorldGenerator worldGenerator;
Camera* camera = new Camera;
Player* player;
Renderer* renderer;
glm::dvec2 cursorPos;
glm::ivec3 selectedCube(0);
//glm::vec3 selectorRayPos(0.);
float selectedCubeNormale;

float resolutionDecrease = 1.;

int block = 1;
int maxBlocks = 4;
glm::ivec3 blocks[] = { glm::ivec3(1,1,0),glm::ivec3(3,0,0), glm::ivec3(7,0,0), glm::ivec3(3,2,0) };

float aspectRatio;

glm::ivec3 origin1(0);
bool breaking;
bool placing;
bool advancedGraphics = false;

int screenState = 0;
ImFont* font;
ImFont* bigFont;

float renderDistanceSteps = 40;
float renderDistanceMeters = 200;
int loadingDistance = 6;
int selectedWorld = 0;
std::string worldName;
std::vector<std::string> worldList;
float playerSpeed;
float fps;

int iframes = 5;

Settings settings;
MeshRenderer* mr;

Shader screenShader;

int width, height;
//glm::vec3 origin = glm::vec3(0., 0., 0.);
//glm::vec3 origin = glm::vec3(90000., 90000., 90000.);
//float speed = 0.55f;
//glm::mat4 rotation = glm::mat3(1.0);



const double fpsLimit = 1.0 / 60.0;
double lastUpdateTime = 0;  // number of seconds since the last loop
double lastFrameTime = 0;   // number of seconds since the last frame

int maxLogTime = 100;
int logTime = 400;


//POLY
glm::vec3 polyPos = glm::vec3(0., 0., -6.);
int vertexCount = 36;


GLuint FBO, texx, rbo;



void changeState()
{
    if (screenState == 0)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        screenState = 1;
    }
    else if (screenState == 1)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        screenState = 0;
    }
}


glm::ivec2 getResolution()
{
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    return glm::ivec2(mode->width, mode->height);
}

// The MAIN function, from here we start the application and run the game loop
int main()
{

    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    glm::ivec2 res = getResolution();
    aspectRatio = (float)res.x / (float)res.y;
    //res = glm::ivec2(1280,1024);
    WIDTH = res.x;
    HEIGHT = res.y;
    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(WIDTH, HEIGHT, "c444", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwGetCursorPos(window, &(cursorPos.y), &cursorPos.x);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    //glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    //glewInit();

    // Define the viewport dimensions
    
    glfwGetFramebufferSize(window, &width, &height);
    //glViewport(0, 0, width/2., height/2.);







    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    font = io.Fonts->AddFontFromFileTTF("PTC55F.ttf", 40.0f);
    bigFont = io.Fonts->AddFontFromFileTTF("PTC55F.ttf", 300.0f);
    bigFont->FontSize = 100;
    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430 core");
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();





    std::cout << "window initialized\n";

    shader = Shader("vertp.glsl", "fragP.glsl");
    screenShader = Shader("vert.glsl", "frag.glsl");
    texture = Texture("tileset.png", 64, 8, true);
    //texture = Texture("texture.png", 1600, 1600, true);
    texture1 = Texture("normset.png", 64, 8, false);
    texture2 = Texture("skysphere.png", 4000, 2000, true);

    std::cout << "shaders and atlas initialized\n";


    GLfloat vertices[] =
    {
        // Позиции          // Текстурные координаты
        1.,  -1., 0,      // Верхний правый
        1.,  1., 0,        // Нижний правый
        -1.,  1., 0,       // Нижний левый
        -1.,  -1., 0,      // Верхний левый
    };
    GLuint indices[] =
    {
        0, 3, 2,
        0, 1, 2
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &FBO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    //glEnableVertexAttribArray(1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    std::cout << "opengl data ready\n";

    //voxels initialise
    loadGame();
    //std::cout << "game loaded\n";
    //startGame();
    //renderer
    //mr->createWorldMesh(world->getMap(), world->getSize().z, world->getSize().y, world->getSize().z);
    //world->getMeshRenderer()->createScreenMesh();
    //startPoly();
    // Game loop
    //world->getMeshRenderer()->prepareFrameBuffer();
    while (!glfwWindowShouldClose(window))
    {

        double now = glfwGetTime();
        double deltaTime = now - lastUpdateTime;

        if ((now - lastFrameTime) >= fpsLimit)
        {

            render();
            //player->update();
            //player->setPosition(glm::vec3(0., 0., -6.));
            //std::cout << player->getPosition().z << std::endl;
            //std::cout << origin1.z << std::endl;
            //renderPoly();
            lastFrameTime = now;

            fps = 1. / deltaTime;
            lastUpdateTime = now;
        }
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

void saveGame()
{
    if (screenState == 1)
    {
        world->saveChunks();
        settings.setPlayerCameraRotation(camera->getRotation());
        settings.setPlayerPosition(player->getPosition() + (glm::vec3)(Chunk::getSize() * world->getChunkOffset()));
        settings.setPlayerRotation(player->getRotation());
        //settings.setPlayerPosition(glm::vec3(0., 0., -6.));
    }
    settings.setLoadingDistance(loadingDistance);
    settings.setRenderDistanceMeters(renderDistanceMeters);
    settings.setRenderDistanceSteps(renderDistanceSteps);
    settings.setPlayerSpeed(playerSpeed);
    settings.saveWorlds(worldList);
    settings.saveSettings();

}

void loadGame()
{
    settings.loadSettings();
    loadingDistance = settings.getLoadingDistance();
    //loadingDistance = 6;
    renderDistanceMeters = settings.getRenderDistanceMeters();
    renderDistanceSteps = settings.getRenderDistanceSteps();
    playerSpeed = settings.getPlayerSpeed();
    //playerSpeed = 100;
    worldList = settings.loadWorlds();
}

void loadWorld()
{
    Database::setWorld(worldList[selectedWorld]);
}

void createNewWorld()
{
    worldList.push_back(worldName);
    Database::setWorld(worldName);
}

void startGame()
{

    renderer = new Renderer(screenShader, shader, texture, texture1, texture2, width / resolutionDecrease, height / resolutionDecrease);

    glm::mat4 prot = settings.getPlayerRotation();
    glm::mat4 pcrot = settings.getPlayerCameraRotation();
    //std::cout << prot.length << std::endl;

    camera = new Camera();
    camera->setRotation(pcrot);
    worldGenerator = WorldGenerator(64);
    std::cout << "world ready\n";
    std::cout << "stage 1\n";
    renderer->start();
    std::cout << "renderer created\n";
    world = new World(loadingDistance * 2, &worldGenerator, renderer->getMeshRenderer());
    std::cout << "world created\n";
    std::cout << "changed\n";
    player = new Player(world, camera, settings.getPlayerPosition());
    player->setRotation(prot);
    player->setSpeed(playerSpeed / 200.f);
    std::cout << "before map update \n";
    //world->updateMapSampler();
    world->startOffthread();
    std::cout << "world map updated\n";
    screenState = 1;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    renderer->getMeshRenderer()->createScreenMesh();
}




void render()
{
    //std::cout << "render\n";
    glfwPollEvents();

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    switch (screenState)
    {
    case 0:
        renderGui();
        break;
    case 1:
        player->update();
        world->update();
        //if (player != nullptr)
            //player->update();
        //renderGame();
        renderPoly();
        renderGui();
        break;
    }
    glfwSwapBuffers(window);
}

void renderGui()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuiIO& io = ImGui::GetIO();
    std::vector<std::string> worlds = worldList;
    const char** w = new const char* [worlds.size()];
    for (int i = 0; i < worlds.size(); i++)
        w[i] = worlds[i].c_str();
    //std::cout << w[0] << "\n";
    switch (screenState)
    {
    case 0:
        //std::cout << "RENDERING GUI";
        ImGui::SetNextWindowPos({ 0,0 });
        ImGui::SetNextWindowSize({ (float)WIDTH,(float)HEIGHT });
        ImGui::Begin("Menu");
        ImGui::Text("WASD to move, F to fly, G for ultra graphics");
        bigFont->FontSize = 400;
        std::cout << "f1\n";
        ImGui::PushFont(bigFont);
        std::cout << "d2\n";
        ImGui::Text("SARATOV CUBES V");
        std::cout << "d3\n";
        ImGui::PopFont();
        ImGui::PushFont(font);
        std::cout << "d4\n";
        ImGui::SliderFloat("render distance steps", &renderDistanceSteps, 10.f, 200.0f);
        ImGui::SliderFloat("render distance meters", &renderDistanceMeters, 10.f, 600.0f);
        ImGui::SliderInt("loading distance in chunks", &loadingDistance, 1.f, 60.0f);
        ImGui::SliderFloat("player speed", &playerSpeed, 0.f, 600.0f);
        ImGui::SliderFloat("resolution decrease (fps up)", &resolutionDecrease, 0.5f, height/8);
        if(worlds.size()>0)
            ImGui::ListBox("worlds", &selectedWorld, const_cast<const char**>(w), worlds.size(), worlds.size());
        ImGui::InputText(std::string({0}).c_str(), &worldName);
        std::cout << "d5\n";
        ImGui::SameLine();
        std::cout << "d6\n";
        if (ImGui::Button("create"))
        {
            createNewWorld();
            startGame();
        }
        //std::cout << "d7\n";
        if (worlds.size() > 0)
            if (ImGui::Button("start"))
            {

                loadWorld();
                startGame();
            }
        std::cout << "d7\n";
        ImGui::PopFont();
        ImGui::End();
        std::cout << "d8\n";
        break;
    case 1:
        //std::cout << "NIGGERS";
        ImGui::SetNextWindowPos({ 0,0 });
        ImGui::SetNextWindowSize({ (float)WIDTH/10,(float)HEIGHT/40 });
        std::string s = "fps: " + std::to_string(fps);
        ImGui::Begin(s.c_str());
        ImGui::Text(s.c_str());
        ImGui::End();
        break;
    }
    // Render dear imgui into screen
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

}

void renderPoly()
{
    glViewport(0, 0, width / resolutionDecrease, height / resolutionDecrease);
    //glfwPollEvents();
    //world->update();
    //player->update();
    if (placing)
        //place();
        player->place();
    if (breaking)
        //attack();
        player->attack();

    player->setMovement(origin1 * ivec3(1, 1, 1));
    player->selectPlaceable(blocks[block]);


    glm::vec4 dir = glm::vec4(0., 0., 1., 0.) * camera->getRotation();
    selectedCube = Physics::raycastCube(camera->getOrigin(), glm::vec3(dir.x, dir.y, dir.z), world->getMap(), world->getSize());
    glm::vec3 selectorRayPos = Physics::raycastCubeRaw(camera->getOrigin(), glm::vec3(dir.x, dir.y, dir.z), world->getMap(), world->getSize());
    glm::ivec3 norm = Physics::getCubeNormale(selectorRayPos, selectedCube);
    selectedCubeNormale = norm.z;

    renderer->renderGame();

    renderer->getGameShader()->use();
    glm::mat4 proj = (glm::perspective(glm::radians(70.0f), aspectRatio, 0.1f, 100000.0f));
    proj = glm::scale(proj, glm::vec3(-1, -1, 1));
    glm::mat4 view = glm::mat4(1.0f);
    view *= glm::scale(camera->getRotation(), glm::vec3(-1.));
    view = glm::translate(view, player->getPosition()*glm::vec3(-1,-1,-1));
    glm::mat4 model = glm::mat4(1.0f);

    renderer->getGameShader()->setMat4(proj, "projection");
    renderer->getGameShader()->setMat4(model, "model");
    renderer->getGameShader()->setMat4(view, "view");
    renderer->getGameShader()->setInt(2, "normAtlas");

    renderer->getGameShader()->setInt(advancedGraphics, "advancedGraphics");
    renderer->getGameShader()->setFloat(renderDistanceSteps, "renderDistanceSteps");
    renderer->getGameShader()->setFloat(renderDistanceMeters, "rendeDistanceMeters");
    renderer->getGameShader()->setVec3(camera->getOrigin(), "origin");
    renderer->getGameShader()->setInt(3, "texAtlas");
    renderer->getGameShader()->setIvec3(selectedCube, "selectedCube");
    renderer->getGameShader()->setFloat(selectedCubeNormale, "selectedCubeNormale");
    glUniform1iv(renderer->getGameShader()->getUniformLocation("chunkOffset"), 288, world->getChunkSamplerOffset());
    //renderer->getScreenShader()->setInt(5, "chunkOffset");

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_3D, world->getMapSampler());

    //glActiveTexture(GL_TEXTURE0 + 5);
    //glBindTexture(GL_TEXTURE_3D, world->getChunkSamplerOffset());

    //renderer->getScreenShader()->setInt(5, "chunkOffset");

    world->debug();
    glViewport(0, 0, width / 1., height / 1.);
    renderer->postRenderGame();
    renderer->getScreenShader()->setInt(0, "screenTexture");
    renderer->getScreenShader()->use();
    renderer->getScreenShader()->setMat4(camera->getRotation(), "rotation");
    renderer->getScreenShader()->setFloat((float)width / (float)height, "aspectRatio");
    renderer->getScreenShader()->setInt(4, "skysphere");

    world->getMeshRenderer()->postRender();
}

void renderGame()
{


    if (placing)
        //place();
        player->place();
    if (breaking)
        //attack();
        player->attack();

    player->setMovement(origin1*ivec3(1,1,1));
    player->selectPlaceable(blocks[block]);


    glm::vec4 dir = glm::vec4(0., 0., 1., 0.) * camera->getRotation();
    selectedCube = Physics::raycastCube(camera->getOrigin(), glm::vec3(dir.x, dir.y, dir.z), world->getMap(), world->getSize());
    glm::vec3 selectorRayPos = Physics::raycastCubeRaw(camera->getOrigin(), glm::vec3(dir.x, dir.y, dir.z), world->getMap(), world->getSize());
    glm::ivec3 norm = Physics::getCubeNormale(selectorRayPos, selectedCube);
    selectedCubeNormale = norm.z;



    GLint originLoc = shader.getUniformLocation("origin");
    glm::vec3 origin = camera->getOrigin();
    glUniform3f(originLoc, origin.x, origin.y, origin.z);

    GLint selCubeLoc = shader.getUniformLocation("selectedCube");
    glUniform3i(selCubeLoc, selectedCube.x, selectedCube.y, selectedCube.z);

    GLint selCubeNormLoc = shader.getUniformLocation("selectedCubeNormale");
    glUniform1f(selCubeNormLoc, selectedCubeNormale);
    //glUniform3i(selCubeNormLoc, selectedCubeNormale.x, selectedCubeNormale.y, selectedCubeNormale.z);

    GLint rLoc = shader.getUniformLocation("rotation");
    glUniformMatrix4fv(rLoc, 1, GL_FALSE, glm::value_ptr(camera->getRotation()));

    GLint textureAtlasLoc = shader.getUniformLocation("texAtlas");
    glUniform1i(textureAtlasLoc, 3);

    GLint normaleAtlasLoc = shader.getUniformLocation("normAtlas");
    glUniform1i(normaleAtlasLoc, 2);

    GLint skysphereLoc = shader.getUniformLocation("skysphere");
    glUniform1i(skysphereLoc, 4);

    GLint mapLoc = shader.getUniformLocation("voxels");
    glUniform1i(mapLoc, 1);


    GLint agLoc = shader.getUniformLocation("advancedGraphics");
    glUniform1i(agLoc, advancedGraphics);

    GLint timeLoc = shader.getUniformLocation("time");
    glUniform1f(timeLoc, glfwGetTime());

    GLint arLoc = shader.getUniformLocation("aspectRatio");
    glUniform1f(arLoc, aspectRatio);

    GLint rdsLoc = shader.getUniformLocation("renderDistanceSteps");
    glUniform1f(rdsLoc, renderDistanceSteps);

    GLint rdmLoc = shader.getUniformLocation("renderDistanceMeters");
    glUniform1f(rdmLoc, renderDistanceMeters);


    shader.use();

    glActiveTexture(GL_TEXTURE0 + 3);
    glBindTexture(GL_TEXTURE_2D, texture.getTexture());

    glActiveTexture(GL_TEXTURE0 + 2);
    glBindTexture(GL_TEXTURE_2D, texture1.getTexture());
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, texture2.getTexture());

    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0 + 1);
    glBindTexture(GL_TEXTURE_3D, world->getMapSampler());
    //tShader.use();
    mr->render();

    glBindVertexArray(0);

    //glDrawArrays(GL_TRIANGLES, 0, 6);
    //glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //glBindVertexArray(0);

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    //std::cout << origin1.z << "\n\n\n\n\n";
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        saveGame(), glfwSetWindowShouldClose(window, GL_TRUE);

    if (screenState == 1) 
    {
        if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
            player->jump();

        if (action == GLFW_REPEAT)
        {
            float sp = 0.2f;
            if (key == GLFW_KEY_W)
                polyPos.z += sp;
            if (key == GLFW_KEY_A)
                polyPos.x -= sp;
            if (key == GLFW_KEY_S)
                polyPos.z -= sp;
            if (key == GLFW_KEY_D)
                polyPos.z += sp;
        }

        if (action == GLFW_PRESS)
        {
            if (key == GLFW_KEY_W)
                origin1.z++;
            if (key == GLFW_KEY_A)
                origin1.x--;
            if (key == GLFW_KEY_S)
                origin1.z--;
            if (key == GLFW_KEY_D)
                origin1.x++;
            if (key == GLFW_KEY_Q)
                origin1.y++;
            if (key == GLFW_KEY_E)
                origin1.y--;
            if (key == GLFW_KEY_F)
                player->setFlight(!player->isFlying()), player->setVelocity(glm::vec3(0.));
            if (key == GLFW_KEY_LEFT_SHIFT)
                player->setShift(true);
        }

        if (action == GLFW_RELEASE)
        {
            if (screenState == 1)
            {
                if (key == GLFW_KEY_G)
                    advancedGraphics = !advancedGraphics;
                if (key == GLFW_KEY_W)
                    origin1.z--;
                if (key == GLFW_KEY_A)
                    origin1.x++;
                if (key == GLFW_KEY_S)
                    origin1.z++;
                if (key == GLFW_KEY_D)
                    origin1.x--;
                if (key == GLFW_KEY_Q)
                    origin1.y--;
                if (key == GLFW_KEY_E)
                    origin1.y++;
                if (key == GLFW_KEY_LEFT_SHIFT)
                    player->setShift(false);
            }
            if (key == GLFW_KEY_P)
                //world->updateMapSampler();
                //settings.addWorld("niggers");
                //settings.worldList.push_back("niggers");
                //world->deb = !world->deb;
                //world->updateMipmap();
                world->reloadImage();
        }

        if (key == GLFW_KEY_LEFT)
        {
            glm::vec4 temp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);// *rotation;
            //rotation = glm::rotate(rotation, glm::radians(3.f), glm::vec3(temp.x, temp.y, temp.z));
            camera->rotate(3.f, temp);
        }
        if (key == GLFW_KEY_RIGHT)
        {
            glm::vec4 temp = glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);// *rotation;
            //rotation = glm::rotate(rotation, glm::radians(-3.f), glm::vec3(temp.x, temp.y, temp.z));
            camera->rotate(-3.f, temp);
        }
        if (key == GLFW_KEY_UP)
        {
            glm::vec4 temp = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);// *rotation;
            //rotation = glm::rotate(rotation, glm::radians(3.f), glm::vec3(temp.x, temp.y, temp.z));
            camera->rotate(3.f, temp);
        }
        if (key == GLFW_KEY_DOWN)
        {
            glm::vec4 temp = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);// *rotation;
            //rotation = glm::rotate(rotation, glm::radians(-3.f), glm::vec3(temp.x, temp.y, temp.z));
            camera->rotate(-3.f, temp);
        }
        //std::cout << "keyboard checked\n";
    }
}

void cursor_position_callback(GLFWwindow* window, double ypos, double xpos)
{
    glm::dvec2 newPos = glm::dvec2(xpos, ypos);
    glm::dvec2 delta = newPos - cursorPos;
    cursorPos = newPos;
    if (screenState == 1)
    {
        if (iframes > 0)
        {
            iframes--;
            return;
        }
        float c = 0.04f;
        float c1 = 1080. / 1920.;
        if (delta.x != 0.f)//-3.f
            player->rotate(-3.f * c * abs(delta.x), glm::vec4(delta.x, 0.0f, 0.0f, 0.0f));
        if (delta.y != 0.f)//-3.f
            player->rotate(-3.f * c * abs(delta.y) * c1, glm::vec4(0.0f, delta.y, 0.0f, 0.0f));
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (screenState == 1)
    {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            breaking = false;
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
            placing = false;

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            breaking = true;
        if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
            placing = true;
    }
    //std::cout << "mouse checked\n";
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (screenState == 1)
    {
        block += glm::sign(yoffset);
        block %= maxBlocks;
    }
}