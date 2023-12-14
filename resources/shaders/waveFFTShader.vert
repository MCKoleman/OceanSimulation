#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aTexCoord;

out VS_OUT {
	vec3 LightDir;
	vec3 HalfDir;
    vec3 Normal;
	vec2 TexCoord;
	float FogFactor;
} vs_out;

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;
uniform Light light;

void main()
{
	vec4 pos = View * Model * vec4(aPos, 1.0);
	vec3 normal = normalize(aNorm);
	gl_Position = Projection * pos;

	vs_out.FogFactor = min(-pos.z * 0.002, 1.0);
	vs_out.LightDir = normalize(vec3(View * vec4(light.position, 1.0) - pos));
	vs_out.Normal = vec3(inverse(transpose(View * Model)) * vec4(normal, 0.0));
	vs_out.HalfDir = vs_out.LightDir + normalize(-vec3(pos));
	vs_out.TexCoord = aTexCoord;
}