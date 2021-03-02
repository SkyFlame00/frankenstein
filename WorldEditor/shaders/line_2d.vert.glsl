#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 a_Color;
layout (location = 2) in float a_Index;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Pt1Model;
uniform mat4 u_Pt2Model;

out vec3 color;

void main()
{
	mat4 model = a_Index == 0.0 ? u_Pt1Model : u_Pt2Model;
	color = a_Color;
	gl_Position = u_Proj * u_View * model * vec4(a_Pos, 1.0);
}
