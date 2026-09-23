#version 330 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

out vec4 FragColor;

struct Light{
	vec3 Position;
	vec3 Color;
};

#define LIGHTNUM 4
uniform Light lights[LIGHTNUM];
uniform vec3 viewPos;
uniform bool inverse_normals;
uniform sampler2D woodTexture;

void main(){
    vec3 color = texture(woodTexture, fs_in.TexCoords).rgb;
    vec3 normal = normalize(fs_in.Normal);
    if (inverse_normals) normal = -normal;
    vec3 result = vec3(0.0);
    
    for(int i = 0; i < LIGHTNUM; i++){
        float distance = length(lights[i].Position - fs_in.FragPos);
        float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * distance * distance);
        // Ambient
        vec3 ambient = 0.3 * color;
        // Diffuse
        vec3 lightDir = normalize(lights[i].Position - fs_in.FragPos);
        float diff = max(dot(lightDir, normal), 0.0);
        // vec3 diffuse = diff * lights[i].Color;
        vec3 diffuse = diff * lights[i].Color * attenuation;
        // Specular
        vec3 viewDir = normalize(viewPos - fs_in.FragPos);
        vec3 reflectDir = reflect(-lightDir, normal);
        float spec = 0.0;
        vec3 halfwayDir = normalize(lightDir + viewDir);  
        spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
        vec3 specular = spec * lights[i].Color * attenuation;    
       
        result += (ambient +  (diffuse + specular)) * color;
    }

    FragColor = vec4(result, 1.0f);
}