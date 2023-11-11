#pragma once
#include "utils.h"
#include "state.h"
#include "windows/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "rendering/scene.h"
#include <map>

/// <summary>
/// 
/// </summary>
class GUIDebugToolsWindow : public IGUIWindow
{
protected:
	State* mState;
	Scene* mScene;
public:
	void Draw() override
	{
		// Don't draw disabled GUI
		if (!mIsEnabled)
			return;

		if (ImGui::Begin("Debug Tools", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text(("Frame Time : " + mState->frameTime + " [" + mState->fps + " FPS]").c_str());
			// Shading settings
			ImGui::Text("Shading");
			// Select shader
			std::unordered_map<std::string, Shader*> shaderList = mScene->GetShaderList();
			std::map<std::string, Shader*> sortedShaders;
			for (auto iter = shaderList.begin(); iter != shaderList.end(); ++iter)
				sortedShaders.emplace(iter->first, iter->second);
			std::string curShader = mScene->GetCurShader();
			if (ImGui::BeginListBox("Shader"))
			{
				for (auto iter = sortedShaders.begin(); iter != sortedShaders.end(); ++iter)
					GUIWindowUtils::Selectable(iter->first, curShader, iter->first);
				ImGui::EndListBox();

				// Only change the shader if it actually changed
				if (mScene->GetCurShader() != curShader)
				{
					mScene->UseShader(curShader);
				}
			}

			ImGui::Separator();
			PPlane* waterPlane = mScene->GetWaterPlane();
			ImGui::Text("Water");
			int divs = GUIWindowUtils::InputInt("Divisions", waterPlane->GetCurDivisions(), 1, 1000);
			bool divEdited = ImGui::IsItemDeactivatedAfterEdit();
			float size = GUIWindowUtils::InputFloat("Size", waterPlane->GetCurSize(), 0.1f, 10000.0f);
			bool sizeEdited = ImGui::IsItemDeactivatedAfterEdit();
			if (divEdited || sizeEdited)
				waterPlane->GenPlane(size, divs);
			mState->waveInterference = GUIWindowUtils::Checkbox("Interference", mState->waveInterference);
		}
		ImGui::End();
	}

	GUIDebugToolsWindow(State* state, Scene* scene, bool isEnabled)
	{
		mType = GUI::DEBUG_TOOLS;
		mState = state;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};