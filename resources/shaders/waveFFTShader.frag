#version 330 core

in VS_OUT {
	vec3 LightDir;
	vec3 HalfDir;
    vec3 Normal;
	vec2 TexCoord;
	float FogFactor;
} fs_in;

struct Material 
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct Light {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform sampler2D water;
uniform vec3 fogColor;

out vec4 FragColor;

//const vec4 fogColor = vec4(0.25, 0.75, 0.65, 1.0);

void main()
{
	vec3 normal = normalize(fs_in.Normal);
	vec3 lightDir = normalize(fs_in.LightDir);
	vec3 halfwayDir = normalize(fs_in.HalfDir);

	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = light.diffuse * material.diffuse * max(dot(normal, lightDir), 0.0);
	vec3 specular = light.specular * material.specular * max(pow(dot(normal, halfwayDir), material.shininess), 0.0);

	vec4 result = vec4(ambient + diffuse + specular, 1.0);
	FragColor = result * (1.0 - fs_in.FogFactor) + vec4(fogColor, 1.0) * fs_in.FogFactor;
}