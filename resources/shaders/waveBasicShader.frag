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
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    int type; // 0: Point, 1: Directional, 2: Spotlight
};

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
    // Get light direction for each type of light
    vec3 lightDir = normalize(-light.direction);
    if (light.type == 0)
    {
        lightDir = normalize(light.position - fs_in.FragPos);
    }

    // Normal in range [-1,1]
    vec3 normal = normalize(fs_in.Normal);

    // Ambient
    vec3 ambient = light.ambient * material.ambient;

    // Diffuse
    vec3 diffuse = light.diffuse * (max(dot(normal, lightDir), 0.0) * material.diffuse);

    // Specular
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    vec3 specular = light.specular * (pow(max(dot(normal, halfwayDir), 0.0), material.shininess) * material.specular);

    // Result
	vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}