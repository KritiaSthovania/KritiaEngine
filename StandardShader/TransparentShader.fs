#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D transparentTexture;

void main()
{             
    vec4 texColor = texture(transparentTexture, TexCoord);
    FragColor = texColor;
}