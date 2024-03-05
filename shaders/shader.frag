#version 410 core

out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;

//uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 color;
uniform vec3 ambient;
uniform vec3 specular;
uniform float specularPow;
uniform float transparency;

struct DirLight {
    vec3 direction;
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
};

uniform DirLight dirLight;

vec3 CalculateDirLight(vec3 normal, vec3 viewDir, vec3 diffuseColor);

void main()
{
    vec3 diffuseColor = color;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalculateDirLight(norm, viewDir, diffuseColor);

    vec3 ambientColor = dirLight.ambient * ambient;
    result += ambientColor;

    FragColor = vec4(result, transparency);
}

vec3 CalculateDirLight(vec3 normal, vec3 viewDir, vec3 diffuseColor)
{
    vec3 lightDir = normalize(dirLight.direction);

    float diffuseStrenght = max(dot(-lightDir, normal), 0.0);
    vec3 diffuse = (diffuseStrenght * diffuseColor) * dirLight.diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float specularStrength = pow(max(dot(-viewDir, reflectDir), 0.0), specularPow);
    vec3 specular = (specularStrength * specular * dirLight.specular);

    vec3 result = diffuse + specular;

    return result;
}