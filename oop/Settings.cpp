#include "Settings.h"


//glm::vec3 Settings::playerPosition = glm::vec3(110., 34., 110.);
//glm::mat4 Settings::playerRotation = glm::mat4(1.);
//glm::mat4 Settings::playerCameraRotation = glm::mat4(1.);
//float Settings::renderDistanceSteps = 40;
//float Settings::renderDistanceMeters = 150;
//int Settings::loadingDistance = 6;


glm::vec3 Settings::getPlayerPosition()
{
    return playerPosition;
}

glm::mat4 Settings::getPlayerRotation()
{
    return playerRotation;
}
glm::mat4 Settings::getPlayerCameraRotation()
{
    return playerCameraRotation;
}

float Settings::getRenderDistanceSteps()
{
    return renderDistanceSteps;
}

float Settings::getRenderDistanceMeters()
{
    return renderDistanceMeters;
}

int Settings::getLoadingDistance()
{
    return loadingDistance;
}

void Settings::saveSettings()
{
    std::string filename = "settings.txt";
    std::ofstream file(filename, std::ios::out | std::ios::binary);


    file.write((char*)(this), sizeof(Settings));

    // Close the file
    file.close();
}

float Settings::getPlayerSpeed()
{
    return playerSpeed;
}

void Settings::loadSettings()
{
    std::string filename = "settings.txt";
    std::ifstream file(filename, std::ios::out | std::ios::binary);
    if (!file.is_open())
    {
        //std::cerr << " failed to open file" << std::endl;
        return;
    }

    file.read((char*)(this), sizeof(Settings));
}

void Settings::setPlayerPosition(glm::vec3 p)
{
    playerPosition = p;
}

void Settings::setPlayerRotation(glm::mat4 m)
{
    playerRotation = m;
}

void Settings::setPlayerCameraRotation(glm::mat4 m)
{
    playerCameraRotation = m;
}

void Settings::setRenderDistanceSteps(int d)
{
    renderDistanceSteps = d;
}

void Settings::setRenderDistanceMeters(int d)
{
    renderDistanceMeters = d;
}

void Settings::setLoadingDistance(int d)
{
    loadingDistance = d;
}

void Settings::setPlayerSpeed(float s)
{
    playerSpeed = s;
}
