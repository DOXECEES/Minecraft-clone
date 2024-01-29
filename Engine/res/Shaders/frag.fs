#version 330 core
in vec2 TexCoord;

out vec4 lineColor;

uniform sampler2D ourTexture;

void main() { lineColor = texture(ourTexture, TexCoord); }
