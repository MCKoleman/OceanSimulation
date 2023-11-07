#pragma once
#include "files/config.h"
#include "files/fileSystem.h"
#include "files/configReader.h"

#include "windows/window.h"

#include "rendering/shader.h"

constexpr unsigned int SCR_WIDTH = 1280;
constexpr unsigned int SCR_HEIGHT = 720;

// Main program loop
int main();

// Loads all shaders
void LoadShaders(std::vector<Shader*>& shaderList, Config* shaderConfig);