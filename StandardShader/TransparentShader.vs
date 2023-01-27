#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec2 aTexCoord;
layout (location = 4) in mat4 instanceModelMatrix;


out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;
out vec3 T;
out vec3 B;
out vec3 N;

layout (std140) uniform matricesVP{
    mat4 view;
    mat4 projection;
};

uniform mat4 model;
uniform mat3 normalMatrix;
uniform bool instancing;

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
    // Gram-Schmidt Process
    T = normalize(vec3(model * vec4(aTangent, 0.0)));
    N = normalize(vec3(model * vec4(aNormal, 0.0)));
    // re-orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);
    // then retrieve perpendicular vector B with the cross product of T and N
    B = cross(T, N);
}