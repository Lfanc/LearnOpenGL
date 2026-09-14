#version 330 core
out vec4 FragColor;


uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform sampler2D texture_reflection1;  // 从 aiTextureType_AMBIENT 加载
uniform samplerCube skybox;         // 天空盒

uniform vec3 cameraPos;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

void main()
{
    // 采样三张 2D 贴图
    vec3 diffuse  = texture(texture_diffuse1,  TexCoords).rgb;
    vec3 specular = texture(texture_specular1, TexCoords).rgb;
    // 反射贴图控制“哪里反射、反射多强”
    float reflectivity = texture(texture_reflection1, TexCoords).r;

    // 计算反射向量
    vec3 I = normalize(FragPos - cameraPos);
    vec3 R = reflect(I, normalize(Normal));
    vec3 reflection = texture(skybox, R).rgb;

    // 简单混合：反射强度由反射贴图控制
    // vec3 color = diffuse + specular + reflection * reflectivity;
    vec3 color = mix(diffuse + specular, reflection, reflectivity);

    FragColor = vec4(color, 1.0);
}