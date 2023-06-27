#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 Normal;

layout (std140) uniform matricesVP{
    mat4 view;
    mat4 projection;
};

uniform mat4 model;
uniform mat3 normalMatrix;

void main()
{
    TexCoord = aTexCoords;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = normalMatrix * aNormal;   

    gl_Position =  projection * view * vec4(FragPos, 1.0);
}