#include <stdlib.h>
#include "Common/esUtil.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "OpenGL/shader.hpp"
#include "OpenGL/camera.hpp"
#include "OpenGL/model.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct
{
    Shader obj;
    Model *model;
    Camera camera;
} UserData;

int Init(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    const char vShaderStr[] =
#include "model.vs"
        ;

    const char fShaderStr[] =
#include "model.fs"
        ;

    // Load the shaders and get a linked program object
    // userData->programObject = esLoadProgram(vShaderStr, fShaderStr);
    userData->obj = Shader(vShaderStr, fShaderStr);

    userData->camera = Camera(glm::vec3(0.0f, 0.0f, 8.0f));

    stbi_set_flip_vertically_on_load(true);
    userData->model = new Model("data/backpack/backpack.obj");
    // userData->model = new Model("data/robot/source/d2f0cff60afc40f5afe79156ec7db657.obj");

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
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

    userData->obj.use();

    // view/projection transformations
    glm::mat4 projection = glm::perspective(glm::radians(userData->camera.Zoom), (float)esContext->width / (float)esContext->height, 0.1f, 100.0f);
    glm::mat4 view = userData->camera.GetViewMatrix();
    userData->obj.setMat4("projection", projection);
    userData->obj.setMat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));     // it's a bit too big for our scene, so scale it down
    userData->obj.setMat4("model", model);
    
    userData->model->Draw(userData->obj);
}

void Shutdown(ESContext *esContext)
{
    UserData *userData = (UserData *)esContext->userData;

    if (userData->model) {
        delete userData->model;
    }
}

extern "C" int esMain(ESContext *esContext)
{
    esContext->userData = malloc(sizeof(UserData));
    memset(esContext->userData, 0, sizeof(UserData));

    esCreateWindow(esContext, "Model test", 800, 600, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

    if (!Init(esContext))
    {
        esLogMessage("[%s: %d] init failed!\n", __FUNCTION__, __LINE__);
        return GL_FALSE;
    }

    esRegisterShutdownFunc(esContext, Shutdown);
    esRegisterDrawFunc(esContext, Draw);

    return GL_TRUE;
}
