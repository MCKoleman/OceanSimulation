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
			auto& waveList = mScene->GetWaveList();
			for (unsigned int i = 0; i < waveList.size(); i++)
			{
				if (GUIWindowUtils::Deselectable("Wave" + std::to_string(i), mSelectedWave, waveList[i]))
				{
					waveList[i]->wavelength = GUIWindowUtils::InputFloat("Wavelength", waveList[i]->wavelength);
					waveList[i]->amplitude = GUIWindowUtils::InputFloat("Amplitude", waveList[i]->amplitude);
					waveList[i]->speed = GUIWindowUtils::InputFloat("Speed", waveList[i]->speed);
					waveList[i]->direction = GUIWindowUtils::InputVec3("Direction", waveList[i]->direction);
				}
			}
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