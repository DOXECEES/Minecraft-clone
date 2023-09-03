#version 330 core
out vec4 lineColor;
in vec2 TexCoord;

uniform vec4 time;
uniform sampler2D ourTexture;

void main()
{
    lineColor = texture(ourTexture, TexCoord);
}