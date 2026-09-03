#version 330 core

in vec3 FragPos; // 当前片段在3D空间中的位置
in vec3 Normal;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // 环境光
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
    
    // 漫反射
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, norm), 0.0f);
    vec3 diffuse = diff * lightColor;

    // 镜面反射
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);
}