#ifndef MAX_TEXTURE_UNITS
#define MAX_TEXTURE_UNITS 16
#endif

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
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, int activeTextureId);

// Variables
out vec4 FragColor;
in vec2 texCoords;
in vec3 color;
in vec3 fragPos;
in vec3 normal;
flat in int isPolygonSelected;
flat in int isUsingColor;
flat in int textureId;
uniform vec3 u_ViewPos;
uniform DirLight u_DirLight;
uniform Material u_Material;
uniform bool u_Selected;
uniform vec3 u_SelectionColor;
uniform int u_TextureMap[MAX_TEXTURE_UNITS * 2];
uniform sampler2D u_Textures[MAX_TEXTURE_UNITS];
uniform vec3 u_UniformColor;
uniform bool u_IsUsingColor;

void main()
{
    int activeTextureId = -1;

    for (int i = 0; i < MAX_TEXTURE_UNITS * 2; i += 2)
    {
        if (u_TextureMap[i] == textureId)
        {
            activeTextureId = i + 1;
            break;
        }
    }

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(u_ViewPos - fragPos);
	vec3 result = CalcDirLight(u_DirLight, norm, viewDir, activeTextureId);

    if (u_Selected || bool(isPolygonSelected))
        FragColor = mix(vec4(result, 1.0), vec4(u_SelectionColor, 1.0), 0.5);
    else
        FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, int activeTextureId)
{
    vec3 lightDir = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), u_Material.shininess);

    // Combine results
    vec3 ambient, diffuse, specular;

    if (u_IsUsingColor)
    {
        ambient  = light.ambient  * u_UniformColor;
        diffuse  = light.diffuse  * diff * u_UniformColor;
        specular = light.specular * spec * u_UniformColor;
    }
    else
    {
        ambient  = light.ambient  * vec3(texture(u_Textures[activeTextureId], texCoords));
        diffuse  = light.diffuse  * diff * vec3(texture(u_Textures[activeTextureId], texCoords));
    }

    return (ambient + diffuse /*+ specular*/);
}
