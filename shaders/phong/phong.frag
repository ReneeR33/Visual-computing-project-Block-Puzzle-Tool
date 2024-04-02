#version 420 core

out vec4 FragColor;

in vec3 FragPos;
in vec4 LightSpaceFragPos;
in vec3 Normal;
in vec2 Texcoords;
in mat3 TBN;

const int NR_SAMPLES_SHADOW_X_LEFT = 1;
const int NR_SAMPLES_SHADOW_X_RIGHT = 1;
const int NR_SAMPLES_SHADOW_Y_BELOW = 1;
const int NR_SAMPLES_SHADOW_Y_ABOVE = 1;

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

layout(binding = 0) uniform sampler2D shadowMap;
layout(binding = 1) uniform sampler2D diffuseTexture;
layout(binding = 2) uniform sampler2D normalTexture;

vec3 CalculateDirLight(vec3 normal, vec3 viewDir, vec3 diffuseColor);
float ShadowCalculation(vec4 fragPosLightSpace);

void main()
{
    // vec3 diffuseColor = color;
    vec3 diffuseColor = color * vec3(texture(diffuseTexture, Texcoords));

    //vec3 norm = normalize(Normal);
    vec3 norm = texture(normalTexture, Texcoords).rgb;
    norm = norm * 2.0 - 1.0;   
    norm = normalize(TBN * norm);

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

    float shadow = ShadowCalculation(LightSpaceFragPos);

    vec3 result = (1.0 - shadow) * (diffuse + specular);

    return result;
}

float ShadowCalculation(vec4 fragPosLightSpace) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    float currentDepth = projCoords.z;

    float bias = max(0.05 * (1.0 - dot(Normal, -dirLight.direction)), 0.003);

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -NR_SAMPLES_SHADOW_X_LEFT; x <= NR_SAMPLES_SHADOW_X_RIGHT; ++x)
    {
        for(int y = -NR_SAMPLES_SHADOW_Y_BELOW; y <= NR_SAMPLES_SHADOW_Y_ABOVE; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }
    }
    shadow /= (1 + NR_SAMPLES_SHADOW_X_LEFT + NR_SAMPLES_SHADOW_X_RIGHT) * (1 + NR_SAMPLES_SHADOW_Y_BELOW + NR_SAMPLES_SHADOW_Y_ABOVE);

    return shadow;
}