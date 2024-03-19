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

    vec3 result = (1.0 - shadow) * diffuse + specular;

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
    float bias = max(0.05 * (1.0 - dot(Normal, -dirLight.direction)), 0.003);
    //float shadow = (currentDepth - bias) > closestDepth  ? 1.0 : 0.0;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;

    return shadow;
}