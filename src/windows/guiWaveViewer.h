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
			wave->wavelength = GUIWindowUtils::InputFloat("Wavelength", wave->wavelength);
			wave->amplitude = GUIWindowUtils::InputFloat("Amplitude", wave->amplitude);
			wave->speed = GUIWindowUtils::InputFloat("Speed", wave->speed);
			wave->direction = GUIWindowUtils::InputFloat("Direction", wave->direction);
			wave->directionRange = GUIWindowUtils::InputFloat("Direction Range", wave->directionRange);
			wave->numWaves = GUIWindowUtils::InputInt("Number of Waves", wave->numWaves);
			wave->amplitudeDampingRate = Clamp(GUIWindowUtils::InputFloat("Amplitude Growth Rate", wave->amplitudeDampingRate), 0.0f, 1.0f);
			wave->frequencyGrowthRate = Clamp(GUIWindowUtils::InputFloat("Frequency Growth Rate", wave->frequencyGrowthRate), 0.0f, 1.5f);
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