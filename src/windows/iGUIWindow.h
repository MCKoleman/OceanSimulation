#pragma once
#include "glIncludes.h"

enum class GUI
{
	NONE = 0,
	DEBUG_TOOLS = 1,
	CAMERA_VIEWER = 2,
	LIGHT_VIEWER = 3
};

class IGUIWindow
{
protected:
	GUI mType = GUI::NONE;
	std::string mName = "";
	glm::vec2 mScreenPos = glm::vec2(0.0f);
	bool mIsEnabled = true;
public:
	// Draws the GUI to the screen
	virtual void Draw() = 0;

	// Returns whether the GUI is enabled
	bool IsEnabled() { return mIsEnabled; }
	// Enables the GUI
	void Enable() { mIsEnabled = true; }
	// Disables the GUI
	void Disable() { mIsEnabled = false; }
	// Toggles the enabled status of the GUI
	bool ToggleEnabled() { mIsEnabled = !mIsEnabled; return mIsEnabled; }

	// Gets the type of the GUI
	GUI GetType() { return mType; }
	// Gets the name of the GUI
	const std::string GetName() { return mName; }
	// Sets the name of the GUI
	void SetName(const std::string& name) { mName = name; }
	// Gets the position of the GUI on the screen
	const glm::vec2 GetPos() { return mScreenPos; }
	// Sets the position of the GUI on the screen
	void SetPos(const glm::vec2& pos) { mScreenPos = pos; }
};