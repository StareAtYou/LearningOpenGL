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
    GLint samplerLoc;

    glm::mat4 transform;
    GLint transformLoc;
    GLfloat angle;

    // Texture handle
    GLuint textureId;

    // VBOs
    GLuint positionVBO;
    GLuint indicesIBO;
} UserData;

GLuint LoadTexture(const char *fileName)
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

int Init(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    const char vShaderStr[] =
        "#version 300 es                          \n"
        "layout (location = 0) in vec3 aPos;      \n"
        "layout (location = 1) in vec3 aColor;    \n"
        "layout (location = 2) in vec2 aTexCoord; \n"
        "out vec3 ourColor;                       \n"
        "out vec2 TexCoord;                       \n"
        "uniform mat4 transform;                  \n"
        "void main()                              \n"
        "{                                        \n"
        "    gl_Position = transform * vec4(aPos, 1.0);       \n"
        "    ourColor = aColor;                   \n"
        "    TexCoord = aTexCoord;                \n"
        "}                                        \n";

    const char fShaderStr[] =
        "#version 300 es                      \n"
        "precision highp float;             \n"
        "out vec4 FragColor;                  \n"
        "in vec3 ourColor;                    \n"
        "in vec2 TexCoord;                    \n"
        "uniform sampler2D ourTexture;        \n"
        "void main()                          \n"
        "{                                    \n"
        "    FragColor = texture(ourTexture, TexCoord); \n"
        "}                                    \n";

    // Load the shaders and get a linked program object
    userData->programObject = esLoadProgram(vShaderStr, fShaderStr);

    // Get the sampler locations
    userData->samplerLoc = glGetUniformLocation(userData->programObject, "s_texture");

    userData->transformLoc = glGetUniformLocation(userData->programObject, "transform");

    // Load the texture
    userData->textureId = LoadTexture("data/container_rgb.jpg");
    if (userData->textureId == 0)
    {
        return FALSE;
    }

    float vertices[] = {
        // positions          // colors           // texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    // Index buffer
    glGenBuffers(1, &userData->indicesIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->indicesIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices),
                 indices, GL_STATIC_DRAW);

    glGenBuffers(1, &userData->positionVBO);
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
    glClear(GL_COLOR_BUFFER_BIT);

    // Use the program object
    glUseProgram(userData->programObject);

    // Load the vertex position
    glBindBuffer(GL_ARRAY_BUFFER, userData->positionVBO);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Bind the index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, userData->indicesIBO);

    // Bind the texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, userData->textureId);

    glUniformMatrix4fv(userData->transformLoc, 1, GL_FALSE, glm::value_ptr(userData->transform));
    
    // Set the sampler texture unit to 0
    glUniform1i(userData->samplerLoc, 0);

    // Draw
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (const void *)NULL);
}

void Shutdown(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    glDeleteBuffers(1, &userData->positionVBO);
    glDeleteBuffers(1, &userData->indicesIBO);

    // Delete texture object
    glDeleteTextures(1, &userData->textureId);

    // Delete program object
    glDeleteProgram(userData->programObject);
}

void Update(ESContext *esContext, float deltaTime)
{
    UserData *userData = (UserData *)esContext->userData;

    // esLogMessage("[%s: %d] deltaTime value is %f\n", __FUNCTION__, __LINE__, deltaTime);
    userData->angle += (deltaTime * 1.0f);

    if (userData->angle >= 360.0f)
    {
        userData->angle -= 360.0f;
    }
    // esLogMessage("[%s: %d] Rotate angle is %f\n", __FUNCTION__, __LINE__, userData->angle);

    // create transformations
    userData->transform = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    userData->transform = glm::translate(userData->transform, glm::vec3(0.0f, -0.0f, 0.0f));
    userData->transform = glm::rotate(userData->transform, userData->angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

extern "C" int esMain(ESContext *esContext)
{
    esContext->userData = malloc(sizeof(UserData));

    esCreateWindow(esContext, "stb_image_test", 640, 480, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

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
