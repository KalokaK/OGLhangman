#version 330 core
out vec4 FragColor;

in vec3 VertexColor;
in vec2 TexCoord;
in vec3 pos;
uniform sampler2D text;

void main()
{
    FragColor = texture(text, TexCoord)  * vec4((pos + 1) / 2, 1.);
} 
