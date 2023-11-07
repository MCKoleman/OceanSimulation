#pragma once
#include "utils.h"
#include "windows/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "rendering/scene.h"
#include "rendering/camera.h"
#include "state.h"
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

			// Camera settings
			ImGui::Separator();
			ImGui::Text("Camera Settings");
			Camera* cam = mScene->GetCamera();
			cam->SetPos(GUIWindowUtils::InputVec3("Position", cam->GetPos()));
			cam->LookAt(GUIWindowUtils::InputVec3("Target", cam->GetTarget()));
			cam->SetPivot(GUIWindowUtils::InputVec3("Pivot", cam->GetPivot()));
			cam->SetOrthSize(GUIWindowUtils::InputFloat("Size", cam->GetOrthSize()));
			cam->SetFOV(GUIWindowUtils::InputFloat("FOV", cam->GetFOV()));
			cam->SetNearClip(GUIWindowUtils::InputFloat("Near Clip", cam->GetNearClip()));
			cam->SetFarClip(GUIWindowUtils::InputFloat("Far Clip", cam->GetFarClip()));
			cam->SetPerspective(GUIWindowUtils::Checkbox("Perspective", cam->IsPerspective()));
			cam->SetWireframe(GUIWindowUtils::Checkbox("Wireframe", cam->IsWireframe()));
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