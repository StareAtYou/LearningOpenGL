R"(

#version 300 es  
                  
precision highp float;  

out vec4 FragColor;

in vec3 Normal;
in vec2 TexCoords;
uniform sampler2D cubeColorSampler;

void main()
{
    vec3 color = texture(cubeColorSampler, TexCoords).rgb;
    FragColor = vec4(color, 1.0f);
}

)" 