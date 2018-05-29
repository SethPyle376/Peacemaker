#version 330 core
out vec3 FragColor;

in vec2 TexCoord;
in vec3 position;


uniform sampler2D myTexture;

void main()
{
    FragColor = vec3(0.0, 0.0, 1.0);
} 