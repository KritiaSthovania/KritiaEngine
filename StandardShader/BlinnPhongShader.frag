#version 450 core
#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 1

layout(location = 0) out vec4 FragColor;
layout(location = 0) in vec3 Normal;
layout(location = 1) in vec3 FragPos;
layout(location = 2) in vec2 TexCoord;
layout(location = 3) in vec4 FragPosLightSpace;
layout(location = 4) in vec3 T;
layout(location = 5) in vec3 B;
layout(location = 6) in vec3 N;

// material

// Point Light
struct PointLight{
    vec3 color;
    vec3 position; 

    float constant;
    float linear;
    float quadratic;

    float ambient;
    float diffuse;
    float specular;

};


// spot light
struct SpotLight{
    vec3 color;
    vec3 position;
    vec3 direction;    
    float constant;
    float linear;
    float quadratic;
    float cutOffCosInner;
    float cutOffCosOuter;
    float ambient;
    float diffuse;
    float specular;
    mat4 spotLightMatrix;

};

layout(binding = 9) uniform Properties{
    uniform vec3 albedo;
    uniform float shininess;
    uniform float heightScale;
    uniform int depthLayers;
    uniform float farPlaneDistance;
    uniform vec3 viewPos;
    uniform bool hasNormalMap;
    uniform bool hasParallaxMap;
};

// main light source (directional)
layout(binding = 10) uniform Lights{
    uniform vec3 mainLightColor;
    uniform vec3 mainLightDirection;
    uniform float ambientIntensity;
    uniform float diffuseIntensity;
    uniform float specularIntensity;
    uniform PointLight pointLights[NR_POINT_LIGHTS];
    uniform SpotLight spotLights[NR_SPOT_LIGHTS];
};

layout(binding = 0) uniform sampler2D mainTexture;
layout(binding = 1) uniform sampler2D specularMap;
layout(binding = 2) uniform sampler2D normalMap;
layout(binding = 4) uniform sampler2D parallaxMap;
layout(binding = 6) uniform sampler2D shadowMap;
layout(binding = 7) uniform sampler2D shadowMapSpot[NR_SPOT_LIGHTS];
layout(binding = 8) uniform samplerCube shadowMapCube[NR_POINT_LIGHTS];

const int pointLightPcfSamples = 20;
vec3 sampleOffsetDirections[pointLightPcfSamples] = vec3[]
(
    vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
    vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
    vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
    vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
    vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

vec2 CalcParallaxMapping(vec2 texCoords, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord, vec3 albedo, int i);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord, vec3 albedo, int i);
float CalcMainShadow(vec4 fragPosLightSpace);
float CalcPointShadow(PointLight light, int i);
float CalcSpotShadow(SpotLight light, int i);

void main()
{
    mat3 TBN = mat3(normalize(T), normalize(B), normalize(N));
    vec3 norm;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDir = -normalize(mainLightDirection);
    if(hasNormalMap){
        norm = texture(normalMap, TexCoord).rgb;
        norm = normalize(norm * 2.0 - 1.0);  
        norm = normalize(TBN * norm);
    }else{
        norm = normalize(Normal);
    }
    vec2 texCoord;
    if(hasParallaxMap){
        texCoord = CalcParallaxMapping(TexCoord, TBN * viewDir);
        if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0){
            discard;
        }
    }else{
        texCoord = TexCoord;
    }

    vec3 ambientComp = ambientIntensity * mainLightColor * albedo * vec3(texture(mainTexture, texCoord));
    float diffuseFactor = max(dot(norm, lightDir), 0.0);
    vec3 diffuseComp = diffuseFactor * diffuseIntensity * mainLightColor * albedo * vec3(texture(mainTexture, texCoord));

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float specularFactor = pow(max(dot(norm, halfwayDir), 0.0), shininess);
    vec3 specularComp = specularFactor * specularIntensity * mainLightColor * vec3(texture(specularMap, texCoord)); 
    float shadow = CalcMainShadow(FragPosLightSpace);  
    vec4 finalColor = vec4((ambientComp + (1 - shadow) * (diffuseComp + specularComp)), 1.0);  
    for(int i = 0; i<NR_POINT_LIGHTS; i++){
        finalColor += vec4(CalcPointLight(pointLights[i], norm, FragPos, viewDir, texCoord, albedo, i), 1.0);
    }
    for(int i = 0; i<NR_SPOT_LIGHTS; i++){
        finalColor += vec4(CalcSpotLight(spotLights[i], norm, FragPos, viewDir, texCoord, albedo, i), 1.0);
    }
    FragColor = finalColor;
}

// parallax occlusion mapping
vec2 CalcParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // calculate the size of each layer
    float layerDepth = 1.0 / depthLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy * heightScale; 
    vec2 deltaTexCoords = P / depthLayers;
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = texture(parallaxMap, currentTexCoords).r;

    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = texture(parallaxMap, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }

    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(parallaxMap, prevTexCoords).r - currentLayerDepth + layerDepth;

    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord, vec3 albedo, int i)
{
    // if there is no point light or the color is black
    if(light.color != vec3(0,0,0)){
        vec3 lightDir = normalize(light.position - fragPos);
        float diff = max(dot(normal, lightDir), 0.0);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(normal, halfwayDir), 0.0), shininess);
        float distance = length(light.position - fragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
        vec3 ambient  = light.ambient * light.color * vec3(texture(mainTexture, texCoord)) * albedo;
        vec3 diffuse  = light.diffuse * light.color * diff * vec3(texture(mainTexture, texCoord)) * albedo;
        vec3 specular = light.specular * light.color * spec * vec3(texture(specularMap, texCoord));
        ambient  *= attenuation;
        diffuse  *= attenuation;
        specular *= attenuation;
        float shadow = CalcPointShadow(light, i); 
        return (ambient + (1 - shadow) * (diffuse + specular));
    }else{
        return vec3(0,0,0);
    }

}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec2 texCoord, vec3 albedo, int i){
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
      
        vec3 ambient  = light.ambient * light.color * vec3(texture(mainTexture, texCoord)) * albedo;
        vec3 diffuse  = light.diffuse * light.color * diff * vec3(texture(mainTexture, texCoord)) * albedo;
        vec3 specular = light.specular * light.color * spec * vec3(texture(specularMap, texCoord));

        ambient  *= intensity * attenuation;
        diffuse  *= intensity * attenuation;
        specular *= intensity * attenuation;
        float shadow = CalcSpotShadow(light, i);
        return (ambient + (1 - shadow) * (diffuse + specular));
    }else{
        return vec3(0,0,0);
    }
}

float CalcMainShadow(vec4 fragPosLightSpace){
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

float CalcSpotShadow(SpotLight light, int i){
    vec4 fragPosLightSpace = light.spotLightMatrix * vec4(FragPos, 1.0);
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float shadow;
    if(projCoords.z > 1.0){
        shadow = 1.0;
    }else{
        float closestDepth = texture(shadowMapSpot[i], projCoords.xy).r;
        float currentDepth = projCoords.z;
        vec2 texelSize = 1.0 / textureSize(shadowMapSpot[i], 0);
        for(int x = -1; x <= 1; ++x)
        {
            for(int y = -1; y <= 1; ++y)
            {
                float pcfDepth = texture(shadowMapSpot[i], projCoords.xy + vec2(x, y) * texelSize).r; 
                // perhaps need bias
                shadow += currentDepth > pcfDepth ? 1.0 : 0.0;        
            }    
        }
        shadow /= 9.0;
    }
    return shadow;
}

float CalcPointShadow(PointLight light, int i){
    float shadow = 0.0;
    vec3 fragToLight = FragPos - light.position;
    float currentDepth = length(fragToLight);
    float viewDistance = length(viewPos - FragPos);
    float diskRadius = (1.0 + (viewDistance / farPlaneDistance)) / 25.0;
    for(int i = 0; i < pointLightPcfSamples; ++i)
    {
        float closestDepth = texture(shadowMapCube[i], fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= farPlaneDistance;   // Undo mapping [0;1]
        // may need bias
        shadow += currentDepth > closestDepth ? 1.0 : 0.0;
    }
    shadow /= float(pointLightPcfSamples);
    return shadow;
}

