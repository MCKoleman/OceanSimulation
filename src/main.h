#pragma once
#include "input.h"
#include "state.h"

#include "files/config.h"
#include "files/fileSystem.h"
#include "files/configReader.h"

#include "windows/window.h"
#include "windows/guiDebugToolsWindow.h"
#include "windows/guiCameraViewer.h"
#include "windows/guiMaterialViewer.h"
#include "windows/guiLightViewer.h"
#include "windows/guiWaveViewer.h"

#include "rendering/shader.h"
#include "rendering/wave.h"
#include "rendering/material.h"
#include "rendering/scene.h"
#include "rendering/pPlane.h"

constexpr unsigned int SCR_WIDTH = 1280;
constexpr unsigned int SCR_HEIGHT = 720;

// Main program loop
int main();

/// <summary>
/// Draws the current scene
/// </summary>
/// <param name="window">Window to draw to</param>
/// <param name="scene">Scene to draw</param>
/// <param name="state">Global state to use</param>
void OpenGLDraw(Window* window, State* state, Scene* scene);

/// <summary>
/// Opens all defined GUIs
/// </summary>
/// <param name="window">Window to load GUIs to</param>
/// <param name="state"></param>
/// <param name="scene"></param>
void LoadGUIs(Window* window, State* state, Scene* scene);

/// <summary>
/// Loads all defined shaders
/// </summary>
/// <param name="scene">Scene to load shaders to</param>
/// <param name="shaderConfig">Config to load shaders from</param>
void LoadShaders(Scene* scene, Config* shaderConfig, Config* config);

/// <summary>
/// Calculates the frames per second
/// </summary>
/// <param name="state">Global state</param>
/// <param name="prevSecond">Time of the previous second</param>
/// <param name="numFrames">Number of frames this second</param>
void CalculateFPS(State* state, double& prevSecond, int& numFrames);

/// <summary>
/// Loads the default testing scene
/// </summary>
/// <param name="scene">Scene to load to</param>
/// <param name="state">Global state</param>
/// <param name="config">Configuration for the scene</param>
void LoadScene(Scene* scene, State* state, Config* config);
