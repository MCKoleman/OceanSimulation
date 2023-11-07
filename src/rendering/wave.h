#pragma once
#include "glIncludes.h"
#include "rendering/shader.h"
#include <string>

/// <summary>
/// Stores information on a wave to be rendered in a shader
/// </summary>
struct Wave
{
	float wavelength;
	float amplitude;
	float speed;
	glm::vec2 direction;

	/// <summary>
	/// Updates the shader with this wave
	/// </summary>
	/// <param name="shader">Shader to update</param>
	/// <param name="waveNum">Number of the wave to render</param>
	void UpdateShader(Shader* shader, int waveNum)
	{
		std::string waveName = "waves[" + std::to_string(waveNum) + "]";
		shader->SetFloat(waveName + ".frequency", 2.0f / wavelength);
		shader->SetFloat(waveName + ".phase", speed * 2.0f / wavelength);
		shader->SetFloat(waveName + ".amplitude", amplitude);
		shader->SetVec2(waveName + ".direction", glm::normalize(direction));
	}

	explicit Wave(float _wavelength, float _amplitude, float _speed, const glm::vec3& _dir)
		: wavelength(_wavelength), amplitude(_amplitude), speed(_speed), direction(glm::vec2(_dir.x, _dir.z))
	{}
};