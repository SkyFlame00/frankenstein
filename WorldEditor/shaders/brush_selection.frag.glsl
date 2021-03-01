#version 330 core

out float RenderId;

uniform float u_RenderId;

void main()
{
	RenderId = u_RenderId;
}
