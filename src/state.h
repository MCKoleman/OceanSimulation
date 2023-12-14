#pragma once
#include "glIncludes.h"
#include "files/config.h"

class State
{
private:
	Config* mConfig = nullptr;
public:
	bool drawGUI = true;
	bool waveInterference = true;
	bool useSumOfSines = true;
	bool useFFT = true;

	std::string fps = "";
	std::string frameTime = "";

	std::string curShader = "";

	// Returns the current config
	Config* GetConfig()
	{
		return mConfig;
	}
	
	State(Config* config)
	{
		curShader = config->GetString("shader.file");
		mConfig = config;
	}
};