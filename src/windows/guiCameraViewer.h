#pragma once
#include "state.h"
#include "windows/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "rendering/scene.h"

class GUICameraViewer : public IGUIWindow
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

		if (ImGui::Begin("Camera Viewer", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
		{
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

	GUICameraViewer(State* state, Scene* scene, bool isEnabled)
	{
		mType = GUI::CAMERA_VIEWER;
		mState = state;
		mScene = scene;
		mIsEnabled = isEnabled;
	}
};