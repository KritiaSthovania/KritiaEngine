#version 450 core
#extension GL_ARB_separate_shader_objects : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;
//layout (location = 4) in mat4 instanceModelMatrix;

layout (location = 0) out vec3 Normal;
layout (location = 1) out vec3 FragPos;
layout (location = 2) out vec4 FragPosLightSpace;
layout (location = 3) out vec2 TexCoord;
layout (location = 4) out vec3 T;
layout (location = 5) out vec3 B;
layout (location = 6) out vec3 N;

//layout (std140, binding = 0) uniform matricesVP{
//    layout(offset = 0) mat4 view;
//    layout(offset = 64) mat4 projection;
//};

layout (set = 0, binding = 0) uniform UniformBufferObject{
    mat4 model;
    mat4 view;
    mat4 projection;
    mat3 normalMatrix;
    mat4 lightSpaceMatrix;
}ubo;


void main()
{
    gl_Position = ubo.projection * ubo.view * ubo.model * vec4(aPos, 1.0);
    FragPos = vec3(ubo.model * vec4(aPos, 1.0));
    Normal = ubo.normalMatrix * aNormal;
    TexCoord = aTexCoord;
    FragPosLightSpace = ubo.lightSpaceMatrix * vec4(FragPos, 1.0);
    // Gram-Schmidt Process
    T = normalize(vec3(ubo.model * vec4(aTangent, 0.0)));
    N = normalize(vec3(ubo.model * vec4(aNormal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    B = cross(T, N);
    
}