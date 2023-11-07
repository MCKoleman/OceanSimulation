#pragma once
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <imgui/imgui_stdlib.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <stb_image.h>
#include <string>

constexpr auto TEXTURE_DIR = "resources/textures/";
constexpr auto FONT_DIR = "resources/fonts/";
constexpr auto MODELS_DIR = "resources/models/";
constexpr auto SHADER_DIR = "resources/shaders/";

constexpr auto CONFIG_JSON = "configs/config.json";
constexpr auto APP_NAME = "Ocean Simulation by Kristian Kolehmainen & Robert Kilkenny";
constexpr auto ICON = "icon256.png";