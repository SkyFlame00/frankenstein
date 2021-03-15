out vec4 FragColor;

in vec3 color;

uniform bool u_BeingCut;

void main()
{
    vec3 outColor = u_BeingCut ? vec3(1.0f, 0.0f, 0.0f) : color;

    FragColor = vec4(outColor, 1.0);
}
