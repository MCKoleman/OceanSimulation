#version 330 core
layout (location = 0) in vec3 aPos;

out VS_OUT {
	vec3 FragPos;
    vec3 Normal;
} vs_out;

struct Wave
{
	float frequency;
	float phase;
	float amplitude;
	vec2 direction;
};

const int NUM_WAVES = 4;

uniform mat4 MVP;
uniform mat4 Model;
uniform mat3 NormalModel;

uniform float curTime;
uniform Wave waves[NUM_WAVES];

float calcWaveOffset(Wave wave, float sinOffset)
{
	return wave.amplitude * sin(sinOffset);
}

vec3 calcWaveTangent(vec2 dir, float dirOffset)
{
	return vec3(1.0, 0.0, dir.x * dirOffset);
}

vec3 calcWaveBitangent(vec2 dir, float dirOffset)
{
	return vec3(0.0, 1.0, dir.y * dirOffset);
}

vec3 calcWaveNormal(Wave wave, float sinOffset)
{
	float dirOffset = wave.frequency * wave.amplitude * cos(sinOffset);
	return cross(calcWaveTangent(wave.direction, dirOffset), calcWaveBitangent(wave.direction, dirOffset));
}

void main()
{
	vec2 pos = vec2(aPos.x, aPos.z);
	float offset = 0.0;
	vec3 normal = vec3(0.0f);
	for (int i = 0; i < NUM_WAVES; i++)
	{
		float sinOffset = dot(waves[i].direction, pos * waves[i].frequency) + curTime * waves[i].phase;
		offset += calcWaveOffset(waves[i], sinOffset);
		normal += calcWaveNormal(waves[i], sinOffset);
	}
	gl_Position = MVP * vec4(aPos.x, aPos.y + offset, aPos.z, 1.0);

	vs_out.FragPos = vec3(Model * vec4(aPos, 1.0));
	vs_out.Normal = normalize(normal);
}