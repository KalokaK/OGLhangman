#version 330 core
out vec4 FragColor;

in vec3 VertexColor;
in vec2 TexCoord;
in vec3 pos;
uniform sampler2D text;

void main()
{
    vec4 texCol = texture(text, TexCoord);
    if (texCol.a <= 0.01) {
        discard;
    }
    FragColor = texCol  * vec4((pos + 1) / 2, 1.);
} 
