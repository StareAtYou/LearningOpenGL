#ifndef __COMMON_H_
#define __COMMON_H_

#include "config.h"
#include "object.h"
#include "OpenGL/shader.hpp"
#include "OpenGL/camera.hpp"


typedef struct
{
    Camera camera;
    glm::vec3 lightPos;

    Shader* objectCommonShader;
    CubeObject* cubeObject;
    PlaneObject* planeObject;

    GLuint container2Texture;
    GLuint woodTexture;
    
    Shader* depthMapShader;
    GLuint depthMapFBO;
    GLuint depthMapTexture;

#ifdef DEPTH_MAP_VISUAL
    QuadObject* quadObject;
    Shader* quadShader;
#endif
} UserData;


void showMat4(const glm::mat4 &mat);

#endif