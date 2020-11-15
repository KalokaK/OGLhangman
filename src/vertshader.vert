#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aCol;
layout (location = 2) in vec3 aTex;

out vec3 VertexColor;
out vec2 TexCoord;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	VertexColor = aCol;
	TexCoord = vec2(aTex.x, aTex.y);
}
