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
uniform mat4 lightSpaceMatrix;

void main()
{
    vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
    vs_out.TexCoords = aTexCoords;
    // vs_out.NormalCoords = transpose(inverse(mat3(model))) * aNormal;
    vs_out.NormalCoords = mat3(model)*aNormal;
    vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

)"