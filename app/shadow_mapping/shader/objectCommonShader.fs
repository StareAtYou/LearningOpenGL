R"(

#version 300 es  
                  
precision highp float;

struct VS_OUT {
    vec3 FragPos;
    vec3 NormalCoords;
    vec2 TexCoords;
    vec4 FragPosLightSpace;
};
in VS_OUT vs_out;

out vec4 FragColor;

uniform sampler2D diffuseSampler;
uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec3 viewPos;

vec3 BlinnPhong(vec3 normal, vec3 fragPos, vec3 lightPos, vec3 lightColor)
{
    // diffuse
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular
    vec3 viewDir = normalize(viewPos - fragPos);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor; 
    
    return diffuse + specular;
}

float ShadowCalculation(vec4 fragPosLightSpace)
{
    float shadow = 0.0;
    
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    {
        shadow = 0.0;
        return shadow;
    }

    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;

    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;

    // calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(vs_out.NormalCoords);
    vec3 lightDir = normalize(lightPos - vs_out.FragPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);

    // check whether current frag pos is in shadow
    shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    // float texelSize = 1.0 / float(textureSize(shadowMap, 0));
    // for(int x = -1; x <= 1; ++x)
    // {
    //     for(int y = -1; y <= 1; ++y)
    //     {
    //         float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
    //         //shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;   
    //         shadow += currentDepth > pcfDepth  ? 1.0 : 0.0;   
    //     }    
    // }
    // shadow /= 9.0;
        
    return shadow;
}

void main()
{
    vec3 color = texture(diffuseSampler, vs_out.TexCoords).rgb;
    vec3 d_s = vec3(0.0);
    vec3 lightColor = vec3(1.0);

    // ambient
    vec3 ambient = 0.8 * lightColor;

    d_s = BlinnPhong(normalize(vs_out.NormalCoords), vs_out.FragPos, lightPos, lightColor);
    
    float shadow = ShadowCalculation(vs_out.FragPosLightSpace);                      
    vec3 lighting = (ambient + (1.0 - shadow) * d_s) * color;

    FragColor = vec4(lighting, 1.0);
}

)" 