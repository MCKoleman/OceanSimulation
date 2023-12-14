#pragma once
#include "state.h"
#include "windows/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "rendering/scene.h"

class GUIWaveViewer : public IGUIWindow
{
protected:
	State* mState = nullptr;
	Scene* mScene = nullptr;
	Wave* mSelectedWave = nullptr;
public:
	void Draw() override
	{
		// Don't draw disabled GUI
		if (!mIsEnabled)
			return;

		if (ImGui::Begin("Wave Viewer", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			auto* wave = mScene->GetWave();
			wave->wavelength = GUIWindowUtils::InputFloat("Wavelength", wave->wavelength, 0.001f, 20.0f);
			wave->amplitude = GUIWindowUtils::InputFloat("Amplitude", wave->amplitude, 0.001f, 2.0f);
			wave->speed = GUIWindowUtils::InputFloat("Speed", wave->speed, 0.1f, 100.0f);
			wave->direction = GUIWindowUtils::InputFloat("Direction", wave->direction, -360.0f, 360.0f);
			wave->directionRange = GUIWindowUtils::InputFloat("Direction Range", wave->directionRange, 0.0f, 360.0f);
			wave->numWaves = GUIWindowUtils::InputInt("Number of Waves", wave->numWaves, 1, 32);
			wave->amplitudeDampingRate = GUIWindowUtils::InputFloat("Amplitude Growth Rate", wave->amplitudeDampingRate, 0.0f, 1.0f);
			wave->frequencyGrowthRate = GUIWindowUtils::InputFloat("Frequency Growth Rate", wave->frequencyGrowthRate, 0.0f, 1.5f);
		}
		ImGui::End();
	}

	GUIWaveViewer(State* state, Scene* scene, bool isEnabled)
	{
		mType = GUI::WAVE_VIEWER;
		mState = state;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};