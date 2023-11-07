#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include "windows/window.h"
#include "rendering/shader.h"
#include "rendering/light.h"
#include "rendering/camera.h"
#include "rendering/mesh.h"
#include "rendering/iEntity.h"
#include "rendering/material.h"
#include "rendering/wave.h"
#include <unordered_map>

class Scene
{
protected:
	std::string mCurShader = "";

	Shader* mShader = nullptr;
	Light* mGlobalLight = nullptr;
	Camera* mMainCamera = nullptr;
	Material* mMaterial = nullptr;
	IEntity* mCurEntity = nullptr;
	Wave* mFirstWave = nullptr;
	State* mState = nullptr;

	std::unordered_map<std::string, Material*> mMaterialList;
	std::unordered_map<std::string, Shader*> mShaderList;
public:
	// Renders the current scene
	void Draw(Window* window, Shader* shader);

	// Activates the shader with the given name for the scene
	void UseShader(const std::string& name = "");

	// Creates a shader for the scene with the given name from the source file of the given name
	Shader* CreateShader(const std::string& name, bool loadGeom);

	// Creates a shader for the scene with the given name, loading it from a different source than the name
	Shader* CreateShader(const std::string& name, const std::string& source, bool loadGeom);

	// Creates a shader for the scene with the given name, loading it from a config
	Shader* CreateShader(const std::string& name, Config* config);

	// Returns the shader with the given name
	Shader* GetShader(const std::string& name);

	// Returns the scene's camera
	Camera* GetCamera() const { return mMainCamera; }

	// Returns the scene's light
	Light* GetLight() const { return mGlobalLight; }

	// Returns the scene's entity
	IEntity* GetEntity() const { return mCurEntity; }

	// Returns the current shader
	std::string GetCurShader() const { return mCurShader; }

	// Returns a reference to the first wave
	Wave* GetWave() const { return mFirstWave; }

	// Returns a reference to the shader list
	const std::unordered_map<std::string, Shader*>& GetShaderList() { return mShaderList; }

	// Returns a reference to the material list
	const std::unordered_map<std::string, Material*>& GetMaterialList() { return mMaterialList; }

	// Sets up the scene's camera with the given options
	void SetCamera(Config* config) { if (mMainCamera != nullptr) { delete mMainCamera; } mMainCamera = new Camera(config); }

	// Sets the scene's camera to the given camera
	void SetCamera(Camera* cam) { if (mMainCamera != nullptr) { delete mMainCamera; } mMainCamera = cam; }

	// Sets the scene's light to the given light
	void SetLight(Light* light) { if (mGlobalLight != nullptr) { delete mGlobalLight; } mGlobalLight = light; }

	// Sets the scene's material
	void SetMaterial(Material* material) { if (mMaterial != nullptr) { delete mMaterial; } mMaterial = material; }

	// Sets the entity to be rendered
	void SetEntity(IEntity* entity) { if (mCurEntity != nullptr) { delete mCurEntity; } mCurEntity = entity; }
	
	// Adds a wave to the list
	void SetWave(Wave* wave) { if (mFirstWave != nullptr) { delete mFirstWave; } mFirstWave = wave; }

	// Returns the projection matrix of the scene's camera
	const glm::mat4& GetProjectionMatrix(float aspect) { return GetCamera()->GetProjection(aspect); }

	// Returns the view matrix of the scene's camera
	const glm::mat4& GetViewMatrix() { return GetCamera()->GetView(); }

	Scene(State* state) 
	: mState(state)
	{}
};
