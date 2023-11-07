#version 330 core
layout (location = 0) in vec3 aPos;

out VS_OUT {
	vec3 FragPos;
    vec3 Normal;
} vs_out;

struct Wave
{
	float wavelength;
	float amplitude;
	float speed;
	vec3 direction;
};

const int NUM_WAVES = 4;

uniform mat4 MVP;
uniform mat4 Model;
uniform mat3 NormalModel;

uniform float curTime;
uniform Wave waves[NUM_WAVES];

float calculateWaveOffset(Wave wave, float time, vec3 pos)
{
	float phase = wave.speed * 2.0 / wave.wavelength;
	return wave.amplitude * sin(dot(wave.direction, pos * wave.wavelength) + time * phase);
}

void main()
{
	float offset = 0.0;
	for (int i = 0; i < NUM_WAVES; i++)
	{
		offset += calculateWaveOffset(waves[i], curTime, aPos);
	}
	gl_Position = MVP * vec4(aPos.x, aPos.y + offset, aPos.z, 1.0);

	vs_out.FragPos = vec3(Model * vec4(aPos, 1.0));
	vs_out.Normal = vec3(0, 1, 0);
}