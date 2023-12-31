#pragma once
#include "glIncludes.h"
#include "state.h"
#include "files/config.h"
#include "files/fileSystem.h"
#include "windows/iGUIWindow.h"
#include <iostream>
#include <unordered_map>
#include <string>

class Window {
private:
	int mWidth;
	int mHeight;
	bool mIsDockInitialized;
	std::string mName;
	GLFWwindow* mWindow;
	ImGuiIO* mIO;

	std::unordered_map<GUI, IGUIWindow*> mGUIList;
public:
	// Opens a OpenGL window with the given name
	// -----------------------------------------
	Window(int width, int height, const std::string& name, Config* config, State* state);

	// Draws all GUIs
	void DrawGUI();

	// Adds the given GUI window
	void AddGUI(IGUIWindow* gui);
	// Gets the GUI with the given type
	IGUIWindow* GetGUI(GUI type);

	// Gets and stores the current window size
	void CalcWindowSize() { glfwGetWindowSize(mWindow, &mWidth, &mHeight); }

	// Returns the GLFW window reference
	GLFWwindow* GetWindow() { return mWindow; }
	// Returns the width of the window, currently
	int GetWidth() { return mWidth; }
	// Returns the height of the window, currently
	int GetHeight() { return mHeight; }
	// Returns the aspect ratio of the screen
	float GetAspect() { return (float)mWidth / (float)mHeight; }
};

// Sets up the ImGui Style
inline void SetupImGuiStyle(bool isDarkStyle, float alphaThreshold);

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void FramebufferSizeCallback(GLFWwindow* window, int width, int height);
