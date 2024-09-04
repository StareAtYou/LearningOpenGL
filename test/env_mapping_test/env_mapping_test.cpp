#include <stdlib.h>
#include "Common/esUtil.h"

#include "OpenGL/shader.hpp"
#include "OpenGL/camera.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <vector>

#define SCR_WIDTH 800
#define SCR_HEIGHT 600

typedef struct 
{
    Shader backgroundShader;
    Shader cubemapShader;

    Camera camera;

    GLuint cubemapTexture;

    GLuint cubemapVAO;
    GLuint cubemapVBO;
    GLuint backgroundVAO;
    GLuint backgroundVBO;
} UserData;

GLuint loadCubeMap(std::vector<std::string> faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrComponents;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}

int Init(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    userData->camera = Camera(glm::vec3(0.0f, 1.0f, 3.0f));

    const char vCubeMapStr[] =
#include "cubemap.vs"
        ;

    const char fCubeMapStr[] =
#include "cubemap.fs"
        ;

    userData->cubemapShader = Shader(vCubeMapStr, fCubeMapStr);

    const char vBackGroundStr[] =
#include "background.vs"
        ;

    const char fBackGroundStr[] =
#include "background.fs"
        ;

    userData->backgroundShader = Shader(vBackGroundStr, fBackGroundStr);

    float cubeVertices[] = {
        // positions          // normals
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f};

    float backgroundVertices[] = {
        // positions
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

    glGenVertexArrays(1, &userData->cubemapVAO);
    glGenBuffers(1, &userData->cubemapVBO);
    glBindVertexArray(userData->cubemapVAO);
    glBindBuffer(GL_ARRAY_BUFFER, userData->cubemapVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));

    glGenVertexArrays(1, &userData->backgroundVAO);
    glGenBuffers(1, &userData->backgroundVBO);
    glBindVertexArray(userData->backgroundVAO);
    glBindBuffer(GL_ARRAY_BUFFER, userData->backgroundVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(backgroundVertices), &backgroundVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    std::vector<std::string> faces{
        "data/skybox/right.jpg",
        "data/skybox/left.jpg",
        "data/skybox/top.jpg",
        "data/skybox/bottom.jpg",
        "data/skybox/front.jpg",
        "data/skybox/back.jpg",
    };
    userData->cubemapTexture = loadCubeMap(faces);

    userData->cubemapShader.use();
    userData->cubemapShader.setInt("skybox", 0);

    userData->backgroundShader.use();
    userData->backgroundShader.setInt("skybox", 0);

    glEnable(GL_DEPTH_TEST);
    return GL_TRUE;
}

void Draw(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    // Set the viewport
    glViewport(0, 0, esContext->width, esContext->height);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw scene as normal
    userData->cubemapShader.use();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = userData->camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(userData->camera.Zoom), (float)esContext->width / (float)esContext->height, 0.1f, 100.0f);
    userData->cubemapShader.setMat4("model", model);
    userData->cubemapShader.setMat4("view", view);
    userData->cubemapShader.setMat4("projection", projection);
    userData->cubemapShader.setVec3("cameraPos", userData->camera.Position);
    // cubes
    glBindVertexArray(userData->cubemapVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, userData->cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    // draw skybox as last
    glDepthFunc(GL_LEQUAL); // change depth function so depth test passes when values are equal to depth buffer's content
    userData->backgroundShader.use();
    view = glm::mat4(glm::mat3(userData->camera.GetViewMatrix())); // remove translation from the view matrix
    userData->backgroundShader.setMat4("view", view);
    userData->backgroundShader.setMat4("projection", projection);
    // skybox cube
    glBindVertexArray(userData->backgroundVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, userData->cubemapTexture);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

void Update(ESContext *esContext, float deltaTime)
{
    UserData *userData = (UserData *)esContext->userData;
}

static bool firstMouse = true;
void KeyEvent(ESContext *esContext, unsigned char wParam, int pointX, int pointY)
{
    UserData *userData = (UserData *)esContext->userData;

    // esLogMessage("[%s: %d] key callback shoot!\n", __FUNCTION__, __LINE__);
    esLogMessage("[INFO] wParam(%c), pointX(%d), pointY(%d)\n", wParam, pointX, pointY);
    float xpos = static_cast<float>(pointX);
    float ypos = static_cast<float>(pointY);
    float lastX = esContext->width / 2.0f;
    float lastY = esContext->height / 2.0f;

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    switch (wParam)
    {
    case 'a':
        break;
    case 'd':
        break;
    case 'w':
        break;
    case 's':
        break;
    case ' ':
        userData->camera.ProcessMouseMovement(xoffset, yoffset);
        break;
    default:
        break;
    }
}

void Shutdown(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    glDeleteVertexArrays(1, &userData->cubemapVAO);
    glDeleteVertexArrays(1, &userData->backgroundVAO);
    glDeleteBuffers(1, &userData->cubemapVBO);
    glDeleteBuffers(1, &userData->backgroundVBO);
}

extern "C" int esMain(ESContext *esContext)
{
    esContext->userData = malloc(sizeof(UserData));

    esCreateWindow(esContext, "Cube Environment Map", SCR_WIDTH, SCR_HEIGHT, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

    if (!Init(esContext))
    {
        esLogMessage("[%s: %d] init failed!\n", __FUNCTION__, __LINE__);
        return GL_FALSE;
    }

    esRegisterShutdownFunc(esContext, Shutdown);
    esRegisterUpdateFunc(esContext, Update);
    esRegisterDrawFunc(esContext, Draw);
    esRegisterKeyFunc(esContext, KeyEvent);

    return GL_TRUE;
}
