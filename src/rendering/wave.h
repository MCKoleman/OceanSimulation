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
	glm::vec3 direction;

	/// <summary>
	/// Updates the shader with this wave
	/// </summary>
	/// <param name="shader">Shader to update</param>
	/// <param name="waveNum">Number of the wave to render</param>
	void UpdateShader(Shader* shader, int waveNum)
	{
		std::string waveName = "waves[" + std::to_string(waveNum) + "]";
		shader->SetFloat(waveName + ".wavelength", wavelength);
		shader->SetFloat(waveName + ".amplitude", amplitude);
		shader->SetFloat(waveName + ".speed", speed);
		shader->SetVec3(waveName + ".direction", direction);
	}

	explicit Wave(float _wavelength, float _amplitude, float _speed, const glm::vec3& _dir)
		: wavelength(_wavelength), amplitude(_amplitude), speed(_speed), direction(_dir)
	{}
};