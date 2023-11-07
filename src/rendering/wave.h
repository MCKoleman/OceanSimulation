#pragma once
#include "glIncludes.h"
#include "rendering/shader.h"
#include "files/config.h"
#include <string>

/// <summary>
/// Stores information on a wave to be rendered in a shader
/// </summary>
struct Wave
{
private:
	std::vector<glm::vec2> directionVecs;
	float prevDir = 0.0f;
	float prevDirRange = 0.0f;

	/// <summary>
	/// Generates new direction vectors based on the base direction
	/// </summary>
	void GenDirectionVecs()
	{
		float dirMin = direction - directionRange;
		float dirMax = direction + directionRange;
		directionVecs.clear();
		for (int i = 0; i < numWaves; i++)
			directionVecs.push_back(DegreesToDir(GetRandFloat(dirMin, dirMax)));

		// Update variables indicating when these direction were generated
		prevDir = direction;
		prevDirRange = directionRange;
	}
public:
	float wavelength = 0.0f;
	float amplitude = 0.0f;
	float speed = 0.0f;
	float direction = 0.0f;
	float directionRange = 0.0f;
	int numWaves = 8;

	/// <summary>
	/// Updates the shader with this wave
	/// </summary>
	/// <param name="shader">Shader to update</param>
	void UpdateShader(Shader* shader)
	{
		float curFrequency = 2.0f / wavelength;
		float curAmplitude = amplitude;

		// Generate consistent direction vectors
		glm::vec2 curDirection = DegreesToDir(direction);
		if (directionVecs.size() != numWaves || direction != prevDir || directionRange != prevDirRange)
			GenDirectionVecs();

		shader->SetInt("numWaves", numWaves);
		for (int i = 0; i < numWaves; i++)
		{
			// Load wave
			curDirection = directionVecs[i];
			std::string waveName = "waves[" + std::to_string(i) + "]";
			shader->SetFloat(waveName + ".frequency", curFrequency);
			shader->SetFloat(waveName + ".phase", speed * curFrequency);
			shader->SetFloat(waveName + ".amplitude", curAmplitude);
			shader->SetVec2(waveName + ".direction", curDirection);

			// Setup next wave
			curFrequency *= 1.18f;
			curAmplitude *= 0.82f;
		}
	}

	explicit Wave(float _wavelength, float _amplitude, float _speed, float _dir, float _dirRange, int _numWaves)
		: wavelength(_wavelength), amplitude(_amplitude), speed(_speed), direction(_dir), directionRange(_dirRange), numWaves(_numWaves)
	{}

	explicit Wave(Config* config)
		: Wave(config->GetFloat("wavelength"), config->GetFloat("amplitude"), config->GetFloat("speed"), 
			config->GetFloat("direction"), config->GetFloat("directionRange"), config->GetInt("numWaves"))
	{}
};