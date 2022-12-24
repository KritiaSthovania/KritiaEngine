#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in mat4 instanceModelMatrix;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoord;

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
}