#ifndef MAX_TEXTURE_UNITS
#define MAX_TEXTURE_UNITS 16
#endif

#define LOOP2(a, n) a(n) a(n + 1)
#define LOOP4(a, n) LOOP2(a, n) LOOP2(a, n + 2)
#define LOOP8(a, n) LOOP4(a, n) LOOP4(a, n + 4)
#define LOOP16(a, n) LOOP8(a, n) LOOP8(a, n + 8)
#define LOOP32(a, n) LOOP16(a, n) LOOP16(a, n + 16)

#define TEXTURE_CONDITION(i) if (activeTextureId == i) { \
            ambient = light.ambient  * vec3(texture(u_Textures[i], texCoords)); \
            diffuse = light.diffuse  * diff * vec3(texture(u_Textures[i], texCoords)); \
        }

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
            activeTextureId = u_TextureMap[i + 1];
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
        #if MAX_TEXTURE_UNITS == 16
            LOOP16(TEXTURE_CONDITION, 0);
        #elif MAX_TEXTURE_UNITS == 32
            LOOP32(TEXTURE_CONDITION, 0);
        #endif
    }

    return (ambient + diffuse /*+ specular*/);
}
