#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in mat4 instanceModelMatrix;

layout (location = 0) out vec3 Normal;
layout (location = 1) out vec3 FragPos;
layout (location = 2) out vec4 FragPosLightSpace;
layout (location = 3) out vec2 TexCoord;
layout (location = 4) out vec3 T;
layout (location = 5) out vec3 B;
layout (location = 6) out vec3 N;

layout (std140, binding = 0) uniform matricesVP{
    layout(offset = 0) mat4 view;
    layout(offset = 64) mat4 projection;
};

layout (binding = 1) uniform Properties{
    uniform mat4 model;
    uniform mat3 normalMatrix;
    uniform bool instancing;
    uniform mat4 lightSpaceMatrix;
};


void main()
{
    if(instancing){
        gl_Position = projection * view * instanceModelMatrix * vec4(aPos, 1.0);
    }else{
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalMatrix * aNormal;
    TexCoord = aTexCoord;
    FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    // Gram-Schmidt Process
    T = normalize(vec3(model * vec4(aTangent, 0.0)));
    N = normalize(vec3(model * vec4(aNormal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    B = cross(T, N);
    
}