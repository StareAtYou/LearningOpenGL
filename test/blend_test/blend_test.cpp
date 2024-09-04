#include <stdlib.h>
#include "Common/esUtil.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "OpenGL/shader.hpp"
#include "OpenGL/camera.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>
#include <vector>
#include <map>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

typedef struct
{
    Shader blendShader;
    Camera camera;

    GLuint cubeVAO;
    GLuint cubeVBO;

    GLuint planeVAO;
    GLuint planeVBO;

    GLuint transparentVAO;
    GLuint transparentVBO;

    GLuint cubeTexture;
    GLuint floorTexture;
    GLuint transparentTexture;
} UserData;

unsigned int loadTexture(char const *path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

int Init(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    const char vBlendShaderStr[] =
#include "blend.vs"
        ;

    const char fBlendShaderStr[] =
#include "blend.fs"
        ;

    // Load the shaders and get a linked program object
    userData->blendShader = Shader(vBlendShaderStr, fBlendShaderStr);

    userData->camera = Camera(glm::vec3(0.0f, 1.0f, 6.0f));

    float cubeVertices[] = {
        // positions          // texture Coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};
    float planeVertices[] = {
        // positions          // texture Coords
        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, 5.0f, 0.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,

        5.0f, -0.5f, 5.0f, 2.0f, 0.0f,
        -5.0f, -0.5f, -5.0f, 0.0f, 2.0f,
        5.0f, -0.5f, -5.0f, 2.0f, 2.0f};
    float transparentVertices[] = {
        // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0f, 1.0f,
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f,

        0.0f, 0.5f, 0.0f, 0.0f, 0.0f,
        1.0f, -0.5f, 0.0f, 1.0f, 1.0f,
        1.0f, 0.5f, 0.0f, 1.0f, 0.0f};

    glGenVertexArrays(1, &userData->cubeVAO);
    glGenBuffers(1, &userData->cubeVBO);
    glBindVertexArray(userData->cubeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, userData->cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

    glGenVertexArrays(1, &userData->planeVAO);
    glGenBuffers(1, &userData->planeVBO);
    glBindVertexArray(userData->planeVAO);
    glBindBuffer(GL_ARRAY_BUFFER, userData->planeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), &planeVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));

    glGenVertexArrays(1, &userData->transparentVAO);
    glGenBuffers(1, &userData->transparentVBO);
    glBindVertexArray(userData->transparentVAO);
    glBindBuffer(GL_ARRAY_BUFFER, userData->transparentVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glBindVertexArray(0);

    userData->cubeTexture = loadTexture("data/resources/textures/marble.jpg");
    userData->floorTexture = loadTexture("data/resources/textures/metal.png");
    userData->transparentTexture = loadTexture("data/resources/textures/window.png");

    // shader configuration
    userData->blendShader.use();
    userData->blendShader.setInt("texture1", 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return GL_TRUE;
}

std::vector<glm::vec3> windows{
    glm::vec3(-1.5f, 0.0f, -0.48f),
    glm::vec3(1.5f, 0.0f, 0.51f),
    glm::vec3(0.0f, 0.0f, 0.7f),
    glm::vec3(-0.3f, 0.0f, -2.3f),
    glm::vec3(0.5f, 0.0f, -0.6f)};

void Draw(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    std::map<float, glm::vec3> sorted;
    for (unsigned int i = 0; i < windows.size(); i++)
    {
        float distance = glm::length(userData->camera.Position - windows[i]);
        sorted[distance] = windows[i];
    }

    // Set the viewport
    glViewport(0, 0, esContext->width, esContext->height);

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw objects
    userData->blendShader.use();
    glm::mat4 projection = glm::perspective(glm::radians(userData->camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 view = userData->camera.GetViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);
    userData->blendShader.setMat4("projection", projection);
    userData->blendShader.setMat4("view", view);

    // cubes
    glBindVertexArray(userData->cubeVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, userData->cubeTexture);
    model = glm::translate(model, glm::vec3(-1.0f, 0.0f, -1.0f));
    userData->blendShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(2.0f, 0.0f, 0.0f));
    userData->blendShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // floor
    glBindVertexArray(userData->planeVAO);
    glBindTexture(GL_TEXTURE_2D, userData->floorTexture);
    model = glm::mat4(1.0f);
    userData->blendShader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    
    // windows (from furthest to nearest)
    glBindVertexArray(userData->transparentVAO);
    glBindTexture(GL_TEXTURE_2D, userData->transparentTexture);
    for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, it->second);
        userData->blendShader.setMat4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
}

void Shutdown(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;
}

static bool firstMouse = true;
static bool isEnableMouse = false;
static float lastX = SCR_WIDTH / 2.0f;
static float lastY = SCR_HEIGHT / 2.0f;
void MouseMoveEvent(ESContext *esContext, int pointX, int pointY)
{
    if (!isEnableMouse)
    {
        return;
    }

    UserData *userData = (UserData *)esContext->userData;

    // esLogMessage("[INFO] pointX(%d), pointY(%d)\n", pointX, pointY);
    float xpos = static_cast<float>(pointX);
    float ypos = static_cast<float>(pointY);

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

    userData->camera.ProcessMouseMovement(xoffset, yoffset);
}

void MouseWheelEvent(ESContext *esContext, int direct)
{
    if (!isEnableMouse)
    {
        isEnableMouse = true;
    }

    UserData *userData = (UserData *)esContext->userData;

    // esLogMessage("[INFO] sroll value (%d)\n", direct);
    userData->camera.ProcessMouseScroll(static_cast<float>(direct) / 100);
}

extern "C" int esMain(ESContext *esContext)
{
    esContext->userData = malloc(sizeof(UserData));
    memset(esContext->userData, 0, sizeof(UserData));

    esCreateWindow(esContext, "Model test", SCR_WIDTH, SCR_HEIGHT, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

    if (!Init(esContext))
    {
        esLogMessage("[%s: %d] init failed!\n", __FUNCTION__, __LINE__);
        return GL_FALSE;
    }

    esRegisterShutdownFunc(esContext, Shutdown);
    esRegisterDrawFunc(esContext, Draw);
    esRegisterMouseFunc(esContext, MouseMoveEvent, MouseWheelEvent);

    return GL_TRUE;
}
