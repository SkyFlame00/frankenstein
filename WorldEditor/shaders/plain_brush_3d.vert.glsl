#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec3 a_Color;
layout (location = 3) in vec3 a_Normal;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec2 texCoords;
out vec3 color;
out vec3 fragPos;
out vec3 normal;

void main()
{
	fragPos = vec3(u_Model * vec4(a_Pos, 1.0));
    normal = mat3(transpose(inverse(u_Model))) * a_Normal;  
	texCoords = a_TexCoords;
	color = a_Color;
	gl_Position = u_Proj * u_View * u_Model * vec4(a_Pos, 1.0);
}
