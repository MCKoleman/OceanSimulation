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

const int MAX_NUM_WAVES = 64;

uniform mat4 MVP;
uniform mat4 Model;
uniform mat3 NormalModel;

uniform float curTime;
uniform int numWaves;
uniform Wave waves[MAX_NUM_WAVES];

// Calculates the height offset of the wave
float calcWaveOffset(Wave wave, float waveForm)
{
	return wave.amplitude * waveForm;
}

// Calculates the tangent vector of the wave
vec3 calcWaveTangent(vec2 dir, float wavePartial)
{
	return vec3(1.0, 0.0, dir.x * wavePartial);
}

// Calculates the bitangent vector of the wave
vec3 calcWaveBitangent(vec2 dir, float wavePartial)
{
	return vec3(0.0, 1.0, dir.y * wavePartial);
}

// Calculates the normal vector of the wave
vec3 calcWaveNormal(Wave wave, float wavePartial)
{
	return cross(calcWaveTangent(wave.direction, wavePartial), calcWaveBitangent(wave.direction, wavePartial));
}

// Main wave shader program
void main()
{
	vec2 pos = vec2(aPos.x, aPos.z);
	float offset = 0.0;
	vec3 normal = vec3(0.0);
	float prevPartial = 0.0;

	int numIters = min(numWaves, MAX_NUM_WAVES);
	for (int i = 0; i < numIters; i++)
	{
		// Calculate wave offset of 
		// H = sum[ a_i * e^(sin(x * w_i + t * p_i) - 1) ]
		// And wave normal of
		// d/dx = d_x * sum[ w_i * a_i * e^(sin(x * w_i + t * p_i) - 1) * cos(x * w_i + t * p_i) ]
		// d/dz = d_z * sum[ w_i * a_i * e^(sin(x * w_i + t * p_i) - 1) * cos(x * w_i + t * p_i) ]
		// Where
		// theta = x * w_i + t * p_i
		// waveForm = e^(sin(theta) - 1)
		// partial = w_i * a_i * waveForm * cos(theta)
		float theta = (dot(waves[i].direction, pos) + prevPartial) * waves[i].frequency + curTime * waves[i].phase;
		float waveForm = exp(sin(theta) - 1);
		float wavePartial = waves[i].frequency * waves[i].amplitude * waveForm * cos(theta);
		offset += calcWaveOffset(waves[i], waveForm);
		normal += calcWaveNormal(waves[i], wavePartial);
		prevPartial += wavePartial;
	}
	gl_Position = MVP * vec4(aPos.x, aPos.y + offset, aPos.z, 1.0);

	vs_out.FragPos = vec3(Model * vec4(aPos, 1.0));
	vs_out.Normal = normalize(normal);
}