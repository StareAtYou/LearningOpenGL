R"(

#version 300 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

struct VS_OUT {
    vec3 FragPos;
    vec3 NormalCoords;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
};

out VS_OUT vs_out;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vs_out.FragPos = aPos;
    vs_out.TexCoords = aTexCoords;
    vs_out.NormalCoords = aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

)"