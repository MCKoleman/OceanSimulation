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

			// Skyboxes
			ImGui::Separator();
			ImGui::Text("Skybox");
			std::unordered_map<std::string, Texture*> textureList = mScene->GetSkyboxTextureList();
			std::map<std::string, Texture*> sortedTextures;
			for (auto iter = textureList.begin(); iter != textureList.end(); ++iter)
				sortedTextures.emplace(iter->first, iter->second);
			std::string curTexture = mScene->GetSkybox()->GetTexture()->name;
			if (ImGui::BeginListBox("Texture"))
			{
				for (auto iter = sortedTextures.begin(); iter != sortedTextures.end(); ++iter)
					GUIWindowUtils::Selectable(iter->first, curTexture, iter->first);
				ImGui::EndListBox();

				// Only change the texture if it actually changed
				if (mScene->GetSkybox()->GetTexture()->name != curTexture)
				{
					mScene->GetSkybox()->SetTexture(mScene->GetSkyboxTexture(curTexture));
				}
			}

			ImGui::Separator();
			ImGui::Text("Water");
			if (mState->useSumOfSines)
			{
				PPlane* waterPlane = mScene->GetWaterPlane();
				int divs = GUIWindowUtils::InputInt("Divisions", waterPlane->GetCurDivisions(), 1, 1000);
				bool divEdited = ImGui::IsItemDeactivatedAfterEdit();
				float size = GUIWindowUtils::InputFloat("Size", waterPlane->GetCurSize(), 0.1f, 10000.0f);
				bool sizeEdited = ImGui::IsItemDeactivatedAfterEdit();
				if (divEdited || sizeEdited)
					waterPlane->GenPlane(size, divs);

				mState->waveInterference = GUIWindowUtils::Checkbox("Interference", mState->waveInterference);
				mState->useSumOfSines = GUIWindowUtils::Checkbox("Sum of Sines", mState->useSumOfSines);
			}
			else
			{
				Ocean* ocean = mScene->GetOcean();
				int divs = GUIWindowUtils::InputInt("Divisions", ocean->GetDimension(), 1, 1000);
				bool divEdited = ImGui::IsItemDeactivatedAfterEdit();

				float size = GUIWindowUtils::InputFloat("Size", ocean->GetLength(), 0.1f, 10000.0f);
				bool sizeEdited = ImGui::IsItemDeactivatedAfterEdit();

				bool geom = GUIWindowUtils::Checkbox("Geometry", ocean->IsGeometry());

				if (divEdited || sizeEdited || geom != ocean->IsGeometry())
					ocean->Generate(divs, size, geom);

				ocean->SetSpectrumHeight(GUIWindowUtils::InputFloat("Spectrum Height", ocean->GetSpectrumHeight(), 0.1f, 10.0f));
				ocean->SetWind(GUIWindowUtils::InputVec2("Wind", ocean->GetWind(), -1.0f, 1.0f));

				mState->useSumOfSines = GUIWindowUtils::Checkbox("Sum of Sines", mState->useSumOfSines);
				mState->useFFT = GUIWindowUtils::Checkbox("Use FFT", mState->useFFT);
			}
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