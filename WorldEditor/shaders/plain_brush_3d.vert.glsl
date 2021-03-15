#version 330 core

layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in vec3 a_Color;
layout (location = 3) in vec3 a_Normal;
layout (location = 4) in float a_IsPolygonSelected;
layout (location = 5) in float a_IsUsingColor;
layout (location = 6) in float a_TextureId;

uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Model;

out vec2 texCoords;
out vec3 color;
out vec3 fragPos;
out vec3 normal;
flat out int isPolygonSelected;
flat out int isUsingColor;
flat out int textureId;

void main()
{
	fragPos = vec3(u_Model * vec4(a_Pos, 1.0));
    normal = mat3(transpose(inverse(u_Model))) * a_Normal;  
	texCoords = a_TexCoords;
	color = a_Color;
	isPolygonSelected = int(a_IsPolygonSelected);
	isUsingColor = int(a_IsUsingColor);
	textureId = int(a_TextureId);
	gl_Position = u_Proj * u_View * u_Model * vec4(a_Pos, 1.0);
}
