#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos; // 当前片段在3D空间中的位置
in vec3 Normal;

struct SpotLight {
    vec3 position;
    vec3  direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform SpotLight spotLight;
uniform vec3 viewPos;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;

void main()
{    
    // FragColor = texture(texture_diffuse1, TexCoords);
    vec3 result = vec3(1.0f);

    vec3 lightVec = spotLight.position - FragPos;
    vec3 lightDir = normalize(lightVec);
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    // 内圈到外圈的过渡
    float theta     = dot(lightDir, normalize(-spotLight.direction));
    float epsilon   = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);   
          
    // 执行光照计算
    float distance = length(lightVec);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + 
                spotLight.quadratic * (distance * distance));

    // 环境光
    vec3 ambient = vec3(texture(texture_diffuse1, TexCoords)) * spotLight.ambient;
    
    // 漫反射
    float diff = max(dot(lightDir, norm), 0.0f);
    vec3 diffuse = vec3(texture(texture_diffuse1, TexCoords)) * diff * spotLight.diffuse;

    // 镜面反射
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0f);
    vec3 specular = vec3(texture(texture_specular1, TexCoords)) * spec * spotLight.specular;

    ambient  *= attenuation; 
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}