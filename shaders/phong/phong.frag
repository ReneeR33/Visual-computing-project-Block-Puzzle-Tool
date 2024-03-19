#version 410 core

out vec4 FragColor;

in vec3 FragPos;
in vec4 LightSpaceFragPos;
in vec3 Normal;

//uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 color;
uniform vec3 ambient;
uniform vec3 specular;
uniform float specularPow;

struct DirLight {
    vec3 direction;
    vec3 diffuse;
    vec3 ambient;
vec3 specular;
};

uniform DirLight dirLight;

uniform sampler2D shadowMap;

vec3 CalculateDirLight(vec3 normal, vec3 viewDir, vec3 diffuseColor);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{
    vec3 diffuseColor = color;

    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = CalculateDirLight(norm, viewDir, diffuseColor);

    vec3 ambientColor = dirLight.ambient * ambient;
    result += ambientColor;

    FragColor = vec4(result, 1.0);
}

vec3 CalculateDirLight(vec3 normal, vec3 viewDir, vec3 diffuseColor)
{
    vec3 lightDir = normalize(dirLight.direction);

    float diffuseStrenght = max(dot(-lightDir, normal), 0.0);
    vec3 diffuse = (diffuseStrenght * diffuseColor) * dirLight.diffuse;

    vec3 reflectDir = reflect(-lightDir, normal);
    float specularStrength = pow(max(dot(-viewDir, reflectDir), 0.0), specularPow);
    vec3 specular = (specularStrength * specular * dirLight.specular);

    float shadow = ShadowCalculation(LightSpaceFragPos);
    if (shadow > 0.1) {
        return vec3(0.0);
    }


    vec3 result = diffuse + specular;

    return result;
}

float ShadowCalculation(vec4 fragPosLightSpace) {
        // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float shadow = currentDepth > closestDepth  ? 1.0 : 0.0;

    return shadow;
}