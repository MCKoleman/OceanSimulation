#pragma once
#include "glIncludes.h"
#include "rendering/shader.h"

/// <summary>
/// Stores information on a material to be rendered in a shader
/// </summary>
struct Material
{
	glm::vec3 kd;
	glm::vec3 ka;
	glm::vec3 ks;
	float ns;

	/// <summary>
	/// Updates the shader with this material
	/// </summary>
	/// <param name="shader">Shader to update</param>
	void UpdateShader(Shader* shader)
	{
		shader->SetVec3("material.diffuse", kd);
		shader->SetVec3("material.ambient", ka);
		shader->SetVec3("material.specular", ks);
		shader->SetFloat("material.shininess", ns);
	}

	explicit Material(const glm::vec3& diffuse, const glm::vec3& ambient, const glm::vec3& specular, float shininess)
		: kd(diffuse), ka(ambient), ks(specular), ns(shininess)
	{}
};