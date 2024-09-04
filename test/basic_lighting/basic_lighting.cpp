#include <stdlib.h>
#include "Common/esUtil.h"
#include "OpenGL/shader.hpp"
#include "OpenGL/camera.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

typedef struct
{
    Shader lighting;
    Shader cube;

    Camera camera;

    GLuint VBO;
    GLuint cubeVAO;
    GLuint lightCubeVAO;
} UserData;

int Init(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    userData->camera = Camera(glm::vec3(0.0f, 1.5f, 6.0f));

    const char vlightingStr[] =
#include "lighting.vs"
        ;

    const char flightingStr[] =
#include "lighting.fs"
        ;

    userData->lighting = Shader(vlightingStr, flightingStr);

    const char vCubeShaderStr[] =
#include "cube.vs"
        ;

    const char fCubeShaderStr[] =
#include "cube.fs"
        ;

    userData->cube = Shader(vCubeShaderStr, fCubeShaderStr);


    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    glGenVertexArrays(1, &userData->cubeVAO);
    glGenBuffers(1, &userData->VBO);
    glBindVertexArray(userData->cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, userData->VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    glGenVertexArrays(1, &userData->lightCubeVAO);
    glBindVertexArray(userData->lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, userData->VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    return GL_TRUE;
}

static glm::vec3 lightPos(0.0f, 1.0f, 0.0f);
void Draw(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    // Set the viewport
    glViewport(0, 0, esContext->width, esContext->height);

    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);

    // Use the program object
    userData->cube.use();
    userData->cube.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
    userData->cube.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    userData->cube.setVec3("lightPos", lightPos);
    userData->cube.setVec3("viewPos", userData->camera.Position);

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(userData->camera.Zoom), (GLfloat)esContext->width / (GLfloat)esContext->height, 0.1f, 100.0f);
    glm::mat4 view = userData->camera.GetViewMatrix();
    userData->cube.setMat4("projection", projection);
    userData->cube.setMat4("view", view);

    // world transformation
    glm::mat4 model = glm::mat4(1.0f);
    userData->cube.setMat4("model", model);

    // render the cube
    glBindVertexArray(userData->cubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    
    // also draw the lamp object
    userData->lighting.use();
    userData->lighting.setMat4("projection", projection);
    userData->lighting.setMat4("view", view);
    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);
    model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
    userData->lighting.setMat4("model", model);
    
    glBindVertexArray(userData->lightCubeVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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

    switch (wParam) {
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

    glDeleteVertexArrays(1, &userData->cubeVAO);
    glDeleteVertexArrays(1, &userData->lightCubeVAO);
    glDeleteBuffers(1, &userData->VBO);
}

extern "C" int esMain(ESContext *esContext)
{
    esContext->userData = malloc(sizeof(UserData));

    esCreateWindow(esContext, "basic_lighting", 800, 600, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

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
