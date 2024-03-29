#version 330 core
#define NR_POINT_LIGHTS 4
#define NR_SPOT_LIGHTS 1

out vec4 FragColor;
in vec2 TexCoord;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;

uniform vec3  albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

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
    samplerCube shadowMapCube;
};

uniform PointLight pointLights[NR_POINT_LIGHTS];

const float PI = 3.14159265359;

vec3 fresnelSchlick(float cosTheta, vec3 F0);
float DistributionGGX(vec3 N, vec3 H, float roughness);
float GeometrySchlickGGX(float NdotV, float roughness);
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

void main(){
	vec3 N = normalize(Normal);
	vec3 V = normalize(viewPos - FragPos);
	vec3 Lo = vec3(0.0);
	for(int i = 0; i < NR_POINT_LIGHTS; i++){
        if(pointLights[i].color != vec3(0.0)){
            vec3 L = normalize(pointLights[i].position - FragPos);
            vec3 H = normalize(V + L);
            float distance = length(pointLights[i].position - FragPos);
            float attenuation = 1.0 / (distance * distance);
            vec3 radiance = pointLights[i].color * attenuation;
            vec3 F0 = vec3(0.04);
            F0 = mix(F0, albedo, metallic);
            vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);
            float NDF = DistributionGGX(N, H, roughness);
            float G = GeometrySmith(N, V, L, roughness);
            
            vec3 nominator = NDF * G * F;
            float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
            vec3 specular = nominator / denominator;

            vec3 ks = F;
            vec3 kd = vec3(1.0) - ks;
            kd *= 1.0 - metallic;

            float NdotL = max(dot(N, L), 0.0);
            Lo += (kd * albedo / PI + specular) * radiance * NdotL;
        }
	}
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color   = ambient + Lo;  
    //color = pow(color, vec3(1.0/2.2)); 
    FragColor = vec4(color, 1.0);
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}  

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}