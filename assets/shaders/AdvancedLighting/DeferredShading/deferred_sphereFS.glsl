#version 330 core
out vec4 FragColor;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec2  ScreenSize;

struct Light {
    vec3  Position;
    vec3  Color;
    float Linear;
    float Quadratic;
    float Radius;      // 球体缩放半径，用来剔除
};
uniform Light light;
uniform vec3 cameraPos;

void main() {
    vec2 TexCoords = gl_FragCoord.xy / ScreenSize;

    vec3  FragPos   = texture(gPosition,   TexCoords).rgb;
    vec3  Normal    = texture(gNormal,     TexCoords).rgb;
    vec3  Diffuse   = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular  = texture(gAlbedoSpec, TexCoords).a;

    // 距离剔除
    vec3  L    = light.Position - FragPos;
    float dist = length(L);
    if (dist > light.Radius || dist < 0.0001)
        discard;

    vec3 lightDir = L / dist;
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 halfway  = normalize(lightDir + viewDir);

    float diff = max(dot(Normal, lightDir), 0.0);

    float spec = 0.0;
    if (diff > 0.0) {
        spec = pow(max(dot(Normal, halfway), 0.0), 16.0);
        spec *= Specular;
    }

    float attenuation = 1.0 / (1.0
                        + light.Linear    * dist
                        + light.Quadratic * dist * dist);

    vec3 lighting = Diffuse * 0.03 + (Diffuse * diff + vec3(spec)) * light.Color * attenuation;

    FragColor = vec4(lighting, 1.0);
}