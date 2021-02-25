#version 330 core

// Structs
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct DirLight {
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  

// Functions declarations
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

// Variables
out vec4 FragColor;
in vec2 texCoords;
in vec3 color;
in vec3 fragPos;
in vec3 normal;
uniform bool u_UsingColor;
uniform vec3 u_ViewPos;
uniform DirLight u_DirLight;
uniform Material u_Material;

void main()
{
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(u_ViewPos - fragPos);

	vec3 result = CalcDirLight(u_DirLight, norm, viewDir);
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    // Combine results
    vec3 ambient, diffuse, specular;

    if (u_UsingColor)
    {
        ambient  = light.ambient  * color;
        diffuse  = light.diffuse  * diff * color;
        specular = light.specular * spec * color;
    }
    else
    {
        ambient  = light.ambient  * vec3(texture(u_Material.diffuse, texCoords));
        diffuse  = light.diffuse  * diff * vec3(texture(u_Material.diffuse, texCoords));
        specular = light.specular * spec * vec3(texture(u_Material.specular, texCoords));
    }

    return (ambient + diffuse /*+ specular*/);
}
