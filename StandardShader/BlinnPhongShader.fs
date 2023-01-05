#version 330 core
#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 1

out vec4 FragColor;
in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
in vec4 FragPosLightSpace;

// material
uniform vec3 albedo;

uniform float shininess;
uniform sampler2D mainTexture;
uniform sampler2D specularMap;
uniform sampler2D shadowMap;

// main light source (directional)
uniform vec3 mainLightColor;
uniform vec3 mainLightDirection;
uniform float ambientIntensity;
uniform float diffuseIntensity;
uniform float specularIntensity;

uniform vec3 viewPos;

// Point Light
struct PointLight{
    vec3 color;
    vec3 position; 

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight{
    vec3 color;
    vec3 position;
    vec3 direction;    
    float constant;
    float linear;
    float quadratic;
    float cutOffCosInner;
    float cutOffCosOuter;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
// spot light

uniform SpotLight spotLights[NR_SPOT_LIGHTS];

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, float shadow);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, float shadow);
float CalcShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

void main()
{
    vec3 norm = normalize(Normal);
    vec3 lightDir = -normalize(mainLightDirection);
    vec3 ambientComp = ambientIntensity * mainLightColor * albedo * vec3(texture(mainTexture, TexCoord));
    float diffuseFactor = max(dot(norm, lightDir), 0.0);
    vec3 diffuseComp = diffuseFactor * diffuseIntensity * mainLightColor * albedo * vec3(texture(mainTexture, TexCoord));
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specularComp = specularFactor * specularIntensity * mainLightColor * vec3(texture(specularMap, TexCoord)); 
    float shadow = CalcShadow(FragPosLightSpace, norm, lightDir);  
    vec4 finalColor = vec4((ambientComp + (1 - shadow) * (diffuseComp + specularComp)), 1.0);  
    for(int i = 0; i<NR_POINT_LIGHTS; i++){
        finalColor += vec4(CalcPointLight(pointLights[i], norm, FragPos, viewDir, albedo, shadow), 1.0);
    }
    for(int i = 0; i<NR_SPOT_LIGHTS; i++){
        finalColor += vec4(CalcSpotLight(spotLights[i], norm, FragPos, viewDir, albedo, shadow), 1.0);
    }

    FragColor = finalColor;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, float shadow)
{
    // if there is no point light or the color is black
    if(light.color != vec3(0,0,0)){
        vec3 lightDir = normalize(light.position - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
        vec3 ambient  = light.ambient * light.color * vec3(texture(mainTexture, TexCoord)) * albedo;
        vec3 diffuse  = light.diffuse * light.color * diff * vec3(texture(mainTexture, TexCoord)) * albedo;
        vec3 specular = light.specular * light.color * spec * vec3(texture(specularMap, TexCoord));
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
        return (ambient + (1 - shadow) * (diffuse + specular));
    }else{
        return vec3(0,0,0);
    }

}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 albedo, float shadow){
    if(light.color != vec3(0,0,0)){
        vec3 lightDir = normalize(light.position - fragPos);
        float theta = dot(lightDir, normalize(-light.direction));
        float epsilon = light.cutOffCosInner - light.cutOffCosOuter;
        float intensity = clamp((theta - light.cutOffCosOuter) / epsilon, 0.0, 1.0);
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));   

        float diff = max(dot(normal, lightDir), 0.0);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
      
        vec3 ambient  = light.ambient * light.color * vec3(texture(mainTexture, TexCoord)) * albedo;
        vec3 diffuse  = light.diffuse * light.color * diff * vec3(texture(mainTexture, TexCoord)) * albedo;
        vec3 specular = light.specular * light.color * spec * vec3(texture(specularMap, TexCoord));

        ambient  *= intensity * attenuation;
        diffuse  *= intensity * attenuation;
        specular *= intensity * attenuation;
        return (ambient + (1 - shadow) * (diffuse + specular));
    }else{
        return vec3(0,0,0);
    }
}

float CalcShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir){
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float shadow;
    if(projCoords.z > 1.0){
        shadow = 1.0;
    }else{
        float closestDepth = texture(shadowMap, projCoords.xy).r;
        float currentDepth = projCoords.z;
        vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
                // perhaps need bias
                shadow += currentDepth > pcfDepth ? 1.0 : 0.0;        
            }    
        }
        shadow /= 9.0;
    }
    return shadow;
}