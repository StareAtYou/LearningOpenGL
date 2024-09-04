#include <stdlib.h>
#include "Common/esUtil.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"

typedef struct
{
    // Handle to a program object
    GLuint programObject;

    // Uniform sampler location
    GLint samplerLoc1;
    GLint samplerLoc2;

    GLuint model;
    GLuint view;
    GLuint projection;
    GLfloat angle;

    // Texture handle
    GLuint textureId1;
    GLuint textureId2;

    // VBOs
    GLuint positionVBO;
    GLuint indicesIBO;
    GLuint VAO;
} UserData;

GLuint LoadRGBTexture(const char *fileName)
{
    GLuint texture;
    int width, height, nrChannels;

    unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
    if (data == NULL)
    {
        esLogMessage("Failed to load texture\n");
        return 0;
    }

    esLogMessage("Load image[%s] (%d, %d), (%d) chnnals\n", fileName, width, height, nrChannels);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return texture;
}

GLuint LoadRGBATexture(const char *fileName)
{
    GLuint texture;
    int width, height, nrChannels;

    unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);
    if (data == NULL)
    {
        esLogMessage("Failed to load texture\n");
        return 0;
    }

    esLogMessage("Load image[%s] (%d, %d), (%d) chnnals\n", fileName, width, height, nrChannels);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    return texture;
}

int Init(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    const char vShaderStr[] =
#include "Coordinate-Systems.vs"
        ;

    const char fShaderStr[] =
#include "Coordinate-Systems.fs"
        ;

    // Load the shaders and get a linked program object
    userData->programObject = esLoadProgram(vShaderStr, fShaderStr);

    // Get the sampler locations
    userData->samplerLoc1 = glGetUniformLocation(userData->programObject, "texture1");
    userData->samplerLoc2 = glGetUniformLocation(userData->programObject, "texture2");

    userData->model = glGetUniformLocation(userData->programObject, "model");
    userData->view = glGetUniformLocation(userData->programObject, "view");
    userData->projection = glGetUniformLocation(userData->programObject, "projection");

    // Load the texture
    userData->textureId1 = LoadRGBTexture("data/container_rgb.jpg");
    if (userData->textureId1 == 0)
    {
        return FALSE;
    }

    userData->textureId2 = LoadRGBATexture("data/awesomeface_rgba.png");
    if (userData->textureId2 == 0)
    {
        return FALSE;
    }

    float vertices[] = {
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

    glGenVertexArrays(1, &userData->VAO);
    glGenBuffers(1, &userData->positionVBO);

    glBindVertexArray(userData->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, userData->positionVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    userData->angle = 0.0f;

    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    return GL_TRUE;
}

void Draw(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    // Set the viewport
    glViewport(0, 0, esContext->width, esContext->height);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    // Use the program object
    glUseProgram(userData->programObject);

    // Load the vertex position
    glBindBuffer(GL_ARRAY_BUFFER, userData->positionVBO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)0);
    glEnableVertexAttribArray(0);

    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Bind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->indicesIBO);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, userData->textureId1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, userData->textureId2);

    // Set the sampler texture unit to 0
    glUniform1i(userData->samplerLoc1, 0);
    glUniform1i(userData->samplerLoc2, 1);

    // create transformations
    glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(45.0f), (GLfloat)esContext->width / (GLfloat)esContext->height, 0.1f, 100.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    // pass transformation matrices to the shader
    glUniformMatrix4fv(userData->projection, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(userData->view, 1, GL_FALSE, glm::value_ptr(view));

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f),
    };

    // render boxes
    glBindVertexArray(userData->VAO);
    for (unsigned int i = 0; i < 10; i++)
    {
        // calculate the model matrix for each object and pass it to shader before drawing
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        // float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(userData->angle), glm::vec3(1.0f, 0.3f, 0.5f));
        glUniformMatrix4fv(userData->model, 1, GL_FALSE, glm::value_ptr(model));

        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
}

void Shutdown(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    glDeleteBuffers(1, &userData->positionVBO);
    glDeleteBuffers(1, &userData->indicesIBO);

    glDeleteVertexArrays(1, &userData->VAO);

    // Delete texture object
    glDeleteTextures(1, &userData->textureId1);
    glDeleteTextures(1, &userData->textureId2);

    // Delete program object
    glDeleteProgram(userData->programObject);
}

void Update(ESContext *esContext, float deltaTime)
{
    UserData *userData = (UserData *)esContext->userData;

    userData->angle += (deltaTime * 40.0f);

    if (userData->angle >= 360.0f)
    {
        userData->angle -= 360.0f;
    }
}

extern "C" int esMain(ESContext *esContext)
{
    esContext->userData = malloc(sizeof(UserData));

    esCreateWindow(esContext, "stb_image_test", 800, 600, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

    if (!Init(esContext))
    {
        esLogMessage("[%s: %d] init failed!\n", __FUNCTION__, __LINE__);
        return GL_FALSE;
    }

    esRegisterShutdownFunc(esContext, Shutdown);
    esRegisterUpdateFunc(esContext, Update);
    esRegisterDrawFunc(esContext, Draw);

    return GL_TRUE;
}
