#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D cubeTexture;


void main()
{ 
    FragColor = texture(cubeTexture, TexCoords);
    // FragColor = vec4(1.0f);
}