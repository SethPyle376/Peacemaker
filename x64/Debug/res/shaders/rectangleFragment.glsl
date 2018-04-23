#version 330 core
out vec3 FragColor;

in vec2 TexCoord;

uniform vec3 color;
uniform sampler2D myTexture;

void main()
{
    FragColor = color;
} 