#pragma once
#include "state.h"
#include "windows/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "rendering/scene.h"

class GUIMaterialViewer : public IGUIWindow
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

		if (ImGui::Begin("Material Viewer", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
			Material* material = mScene->GetMaterial();
			material->kd = Clamp(GUIWindowUtils::InputVec3("Diffuse", material->kd), 0.0f, 1.0f);
			material->ka = Clamp(GUIWindowUtils::InputVec3("Ambient", material->ka), 0.0f, 1.0f);
			material->ks = Clamp(GUIWindowUtils::InputVec3("Specular", material->ks), 0.0f, 1.0f);
			material->ns = Clamp(GUIWindowUtils::InputFloat("Shininess", material->ns), 0.0f, 64.0f);
		}
		ImGui::End();
	}

	GUIMaterialViewer(State* state, Scene* scene, bool isEnabled)
	{
		mType = GUI::MATERIAL_VIEWER;
		mState = state;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};