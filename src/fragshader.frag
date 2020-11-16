#version 330 core
out vec4 FragColor;

in vec3 VertexColor;
in vec2 TexCoord;

uniform sampler2D text;

void main()
{
    FragColor = texture(text, TexCoord) * vec4(VertexColor, 1.);
} 
