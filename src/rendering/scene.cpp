#include "scene.h"

// Renders the current scene
void Scene::Draw(Window* window, Shader* shader)
{
	// Calculate camera view projection
	window->CalcWindowSize();
	glm::mat4 viewProj = GetProjectionMatrix(window->GetAspect()) * GetViewMatrix();

	// Draw all of the scene
	shader->Use();
	shader->SetFloat("curTime", (float)glfwGetTime());
	shader->SetVec3("viewPos", GetCamera()->GetPos());
	shader->SetBool("interference", mState->waveInterference);
	mGlobalLight->UpdateShader(shader);
	mMaterial->UpdateShader(shader);
	mFirstWave->UpdateShader(shader);
	
	if (mWaterPlane != nullptr)
		mWaterPlane->Draw(shader, mState, viewProj, false);
}

// Activates the shader with the given name for the scene
void Scene::UseShader(const std::string& name)
{
	if (mShaderList.find(name) != mShaderList.end() && mShaderList[name] != nullptr)
	{
		mCurShader = name;
		mShader = mShaderList[name];
		mShader->Use();
	}
}

// Activates the cubebox shader 
void Scene::UseSkybox(const std::string& name)
{
	mShader = mShaderList[name];
	glDepthMask(GL_FALSE);
	mShader->Use();
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mCubemap->id);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);

}

// Creates a shader for the scene with the given name from the source file of the given name
Shader* Scene::CreateShader(const std::string& name, bool loadGeom)
{
	if (mShaderList.find(name) == mShaderList.end())
	{
		Shader* newShader = new Shader(name, loadGeom);
		newShader->name = name;
		mShaderList.emplace(name, newShader);
		return newShader;
	}
	return mShaderList[name];
}

// Creates a shader for the scene with the given name, loading it from a different source than the name
Shader* Scene::CreateShader(const std::string& name, const std::string& source, bool loadGeom)
{
	if (mShaderList.find(name) == mShaderList.end())
	{
		Shader* newShader = new Shader(source, loadGeom);
		newShader->name = name;
		mShaderList.emplace(name, newShader);
		return newShader;
	}
	return mShaderList[name];
}

// Creates a shader for the scene with the given name, loading it from a config
Shader* Scene::CreateShader(const std::string& name, Config* config)
{
	if (mShaderList.find(name) == mShaderList.end())
	{
		Shader* newShader = new Shader(config->GetString("file"), config->GetBool("hasGeomShader"));
		newShader->name = name;
		mShaderList.emplace(name, newShader);
		return newShader;
	}
	return mShaderList[name];
}

// Returns the shader with the given name
Shader* Scene::GetShader(const std::string& name)
{
	if (mShaderList.find(name) != mShaderList.end())
		return mShaderList[name];
	return nullptr;
}
