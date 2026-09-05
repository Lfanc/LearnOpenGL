#version 330 core

in vec3 FragPos; // 当前片段在3D空间中的位置
in vec3 Normal;
in vec2 TexCoords;

out vec4 FragColor;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float     shininess;
}; 
struct Light {
    // vec3 position; // 使用定向光就不再需要了
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform Light light;
uniform Material material;

uniform vec3 viewPos;

void main()
{
    // 环境光
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;
    
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(lightDir, norm), 0.0f);
    vec3 diffuse = vec3(texture(material.diffuse, TexCoords)) * diff * light.diffuse;

    // 镜面反射
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = vec3(texture(material.specular, TexCoords)) * spec * light.specular;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}