#pragma once
#include "state.h"
#include "windows/iGUIWindow.h"
#include "windows/guiWindowUtils.h"
#include "rendering/scene.h"

class GUILightViewer : public IGUIWindow
{
protected:
    State* mState;
    Scene* mScene;
public:
    void Draw() override
    {
        if (ImGui::Begin("Light Viewer", &mIsEnabled, ImGuiWindowFlags_AlwaysAutoResize))
        {
            Light* light = mScene->GetLight();
            LightType type = light->GetType();

            // Select light type
            if (ImGui::BeginListBox("Type"))
            {
                GUIWindowUtils::Selectable("Point", type, LightType::POINT);
                GUIWindowUtils::Selectable("Directional", type, LightType::DIR);
                GUIWindowUtils::Selectable("Spot", type, LightType::SPOT);
                light->SetType(type);
                ImGui::EndListBox();
            }

            // Light position
            if (type == LightType::SPOT || type == LightType::POINT)
            {
                light->SetOffset(GUIWindowUtils::InputVec3("Position", light->GetOffset()));
            }

            // Light direction
            if (type == LightType::SPOT || type == LightType::DIR)
            {
                light->SetDir(GUIWindowUtils::InputVec3("Direction", light->GetDir()));
            }

            // Light Diffuse, Ambient, and Specular
            light->SetKD(GUIWindowUtils::InputFloat("Diffuse", light->GetKD()));
            light->SetKA(GUIWindowUtils::InputFloat("Ambient", light->GetKA()));
            light->SetKS(GUIWindowUtils::InputFloat("Specular", light->GetKS()));

            // Light gamma
            light->SetGamma(GUIWindowUtils::Checkbox("Gamma", light->GetGamma()));

            // Show attributes of point and spotlights
            if (type == LightType::POINT || type == LightType::SPOT)
            {
                // Range coefficient
                light->SetRange(GUIWindowUtils::InputFloat("Range", light->GetRange()));
            }
            // Show attributes of spotlight
            if (type == LightType::SPOT)
            {
                // Inner radius
                light->SetSpotInnerRadius(GUIWindowUtils::InputFloat("Inner Radius", light->GetSpotInnerRadius()));
                // Outer radius
                light->SetSpotOuterRadius(GUIWindowUtils::InputFloat("Outer Radius", light->GetSpotOuterRadius()));
            }

            // Light color
            light->SetColor(GUIWindowUtils::ColorPicker("Light Color", light->GetColor()));

        }
        ImGui::End();
    }

    GUILightViewer(State* state, Scene* scene, bool isEnabled)
    {
        mType = GUI::LIGHT_VIEWER;
        mIsEnabled = isEnabled;
        mState = state;
        mScene = scene;
    }
};