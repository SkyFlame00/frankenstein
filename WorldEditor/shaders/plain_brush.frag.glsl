#version 330 core

out vec4 FragColor;

in vec2 texCoords;
in vec3 color;

uniform bool u_UsingColor;

void main()
{
	if (u_UsingColor)
	{
		FragColor = vec4(color, 1.0);
	}
}