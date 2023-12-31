#pragma once
#include "glIncludes.h"
#include "files/config.h"
#include "windows/window.h"
#include "rendering/shader.h"
#include "rendering/light.h"
#include "rendering/camera.h"
#include "rendering/mesh.h"
#include "rendering/iEntity.h"
#include "rendering/pPlane.h"
#include "rendering/material.h"
#include "rendering/wave.h"
#include "rendering/cubemap.h"
#include "rendering/ocean.h"
#include <unordered_map>

class Scene
{
protected:
	std::string mCurShader = "";

	Shader* mShader = nullptr;
	Light* mGlobalLight = nullptr;
	Camera* mMainCamera = nullptr;
	Material* mMaterial = nullptr;
	PPlane* mWaterPlane = nullptr;
	Wave* mFirstWave = nullptr;
	State* mState = nullptr;
	Cubemap* mSkybox = nullptr;
	Ocean* mOcean = nullptr;

	std::unordered_map<std::string, Texture*> mSkyboxTextureList;
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

	// Returns the skybox texture with the given name
	Texture* GetSkyboxTexture(const std::string name)
	{
		if (mSkyboxTextureList.find(name) != mSkyboxTextureList.end())
			return mSkyboxTextureList[name];
		return nullptr;
	}

	// Returns the scene's camera
	Camera* GetCamera() const { return mMainCamera; }

	// Returns the scene's light
	Light* GetLight() const { return mGlobalLight; }

	// Returns the scene's skybox
	Cubemap* GetSkybox() const { return mSkybox; }

	// Returns the scene's ocean
	Ocean* GetOcean() const { return mOcean; }

	// Returns the scene's plane
	PPlane* GetWaterPlane() const { return mWaterPlane; }

	// Returns the current shader
	std::string GetCurShader() const { return mCurShader; }

	// Returns a reference to the first wave
	Wave* GetWave() const { return mFirstWave; }

	// Returns a reference to the material list
	Material* GetMaterial() { return mMaterial; }

	// Returns a reference to the shader list
	const std::unordered_map<std::string, Shader*>& GetShaderList() { return mShaderList; }

	// Returns a reference to the skybox texture list
	const std::unordered_map<std::string, Texture*>& GetSkyboxTextureList() { return mSkyboxTextureList; }

	// Sets up the scene's camera with the given options
	void SetCamera(Config* config) { if (mMainCamera != nullptr) { delete mMainCamera; } mMainCamera = new Camera(config); }

	// Sets the scene's camera to the given camera
	void SetCamera(Camera* cam) { if (mMainCamera != nullptr) { delete mMainCamera; } mMainCamera = cam; }

	// Sets the skybox to the given cubemap
	void SetSkybox(Cubemap* skybox) { if (mSkybox != nullptr) { delete mSkybox; } mSkybox = skybox; }

	// Sets the scene's light to the given light
	void SetLight(Light* light) { if (mGlobalLight != nullptr) { delete mGlobalLight; } mGlobalLight = light; }

	// Sets the scene's material
	void SetMaterial(Material* material) { if (mMaterial != nullptr) { delete mMaterial; } mMaterial = material; }

	// Sets the plane to be rendered
	void SetWaterPlane(PPlane* plane) { if (mWaterPlane != nullptr) { delete mWaterPlane; } mWaterPlane = plane; }
	
	// Adds a wave to the list
	void SetWave(Wave* wave) { if (mFirstWave != nullptr) { delete mFirstWave; } mFirstWave = wave; }

	// Sets the ocean of the scene
	void SetOcean(Ocean* ocean) { if (mOcean != nullptr) { delete mOcean; } mOcean = ocean; }

	// Adds the given skybox texture
	void AddSkyboxTexture(Texture* texture) { mSkyboxTextureList.try_emplace(texture->name, texture); }

	// Returns the projection matrix of the scene's camera
	const glm::mat4& GetProjectionMatrix(float aspect) { return GetCamera()->GetProjection(aspect); }

	// Returns the view matrix of the scene's camera
	const glm::mat4& GetViewMatrix() { return GetCamera()->GetView(); }

	Scene(State* state) 
	: mState(state)
	{}
};
