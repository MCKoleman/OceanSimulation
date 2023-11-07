#version 330 core
out vec4 FragColor;

in VS_OUT {
	vec3 FragPos;
    vec3 Normal;
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

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    // Normal in range [-1,1]
    vec3 normal = normalize(fs_in.Normal);

    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    vec3 lightDir = normalize(light.position - fs_in.FragPos);
    vec3 diffuse = light.diffuse * (max(dot(normal, lightDir), 0.0) * material.diffuse);

    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 specular = light.specular * (pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * material.specular);

    // Result
	vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}