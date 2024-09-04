#include <stdlib.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "OpenGL/model.hpp"

#include <iostream>
#include "common.h"
#include "WinEvent.h"
#include "log.h"


static float cube_vertices[] = {
    // positions            // normals         // texcoords
    // back face
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
    1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
    1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,  // bottom-right
    1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,   // top-right
    -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
    -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,  // top-left
    // front face
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
    1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
    1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // top-right
    -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
    -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
    // left face
    -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
    -1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,  // top-left
    -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
    -1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
    -1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,   // top-right
                                                        // right face
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
    1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
    1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,    // top-right
    1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,   // bottom-right
    1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,     // top-left
    1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,    // bottom-left
    // bottom face
    -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
    1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,  // top-left
    1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
    1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,   // bottom-left
    -1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
    -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
    // top face
    -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,  // top-right
    1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,   // bottom-right
    -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
    -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f   // bottom-left
};

static float plane_vertices[] = {
    // positions            // normals         // texcoords
    25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
    -25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,

    25.0f, -0.5f, 25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 0.0f,
    -25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 0.0f, 25.0f,
    25.0f, -0.5f, -25.0f, 0.0f, 1.0f, 0.0f, 25.0f, 25.0f};

#ifdef DEPTH_MAP_VISUAL
static float quad_vertices[] = {
    // positions        // texture Coords
    -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
    1.0f, -1.0f, 0.0f, 1.0f, 0.0f};
#endif

static UserData *gUserData = nullptr;
static Model* gModel = nullptr;
static float near_plane = 1.0f, far_plane = 7.5f;

static void initShader()
{
    /* init depth map shader */
    const char vDepthMapShaderStr[] =
#include "shader/depthMap.vs"
        ;
    const char fDepthMapShaderStr[] =
#include "shader/depthMap.fs"
        ;   
    gUserData->depthMapShader = new Shader(vDepthMapShaderStr, fDepthMapShaderStr);


    /* init object common shader */
    const char vObjectCommonShaderStr[] =
#include "shader/objectCommonShader.vs"
        ;
    const char fObjectCommonShaderStr[] =
#include "shader/objectCommonShader.fs"
        ;
    gUserData->objectCommonShader = new Shader(vObjectCommonShaderStr, fObjectCommonShaderStr);


#ifdef DEPTH_MAP_VISUAL
    const char vQuadShaderStr[] =
#include "shader/quad.vs"
        ;
    const char fQuadShaderStr[] =
#include "shader/quad.fs"
        ;
    gUserData->quadShader = new Shader(vQuadShaderStr, fQuadShaderStr);
#endif

}

static void initObjectsMVP()
{
    CubeObject *cube = gUserData->cubeObject;
    PlaneObject *plane = gUserData->planeObject;

    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    /* cube MVP */
    glm::mat4 cubeModels[CUBE_OBJECT_NUM];
    for (int i = 0; i < CUBE_OBJECT_NUM; i++) {
        cubeModels[i] = glm::mat4(1.0f);
    }

    cubeModels[0] = glm::translate(cubeModels[0], glm::vec3(0.0f, 0.0f, -3.0f));
    // cubeModels[0] = glm::rotate(cubeModels[0], glm::radians(45.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
    cubeModels[0] = glm::scale(cubeModels[0], glm::vec3(0.5f));

    cubeModels[1] = glm::translate(cubeModels[1], glm::vec3(1.5f, 0.0f, 0.0));
    // cubeModels[1] = glm::rotate(cubeModels[1], glm::radians(90.0f), glm::normalize(glm::vec3(0.0f, 1.0f, 1.0f)));
    cubeModels[1] = glm::scale(cubeModels[1], glm::vec3(0.5f));

    cubeModels[2] = glm::translate(cubeModels[2], glm::vec3(-1.5f, 0.0f, 0.0));
    // cubeModels[2] = glm::rotate(cubeModels[2], glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
    cubeModels[2] = glm::scale(cubeModels[2], glm::vec3(0.5f));

    for (int i = 0; i < CUBE_OBJECT_NUM; i++) {
        // cube[i].setMVP(cubeModels[i], view, projection);
        cube[i].setModelMatrix(cubeModels[i]);
    }

    /* plane MVP */
    glm::mat4 planeModels[PLANE_OBJECT_NUM];
    for (int i = 0; i < PLANE_OBJECT_NUM; i++) {
        planeModels[i] = glm::mat4(1.0f);
    }
    // planeModels[0] = glm::translate(planeModels[0], glm::vec3(0.0f, -1.0f, 0.0f));
    // planeModels[0] = glm::rotate(planeModels[0], glm::radians(10.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f)));
    // planeModels[0] = glm::scale(planeModels[0], glm::vec3(0.5f));
    for (int i = 0; i < PLANE_OBJECT_NUM; i++) {
        // plane[i].setMVP(planeModels[i], view, projection);
        plane[i].setModelMatrix(planeModels[i]);
    }
}

static void initModelMVP(Shader* shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(-0.8f, 0.0f, -1.5f));
    model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    // model = glm::rotate(model, glm::radians(45.0f), glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f)));
    model = glm::scale(model, glm::vec3(0.3f));

    shader->use();
    shader->setMat4("model", model);
}

static void initTexture()
{
    CubeObject *cube = gUserData->cubeObject;
    PlaneObject *plane = gUserData->planeObject;
    
    glGenTextures(1, &gUserData->container2Texture);
    glGenTextures(1, &gUserData->woodTexture);

    BaseObject::LoadTexture(gUserData->container2Texture, "data/resources/textures/container2.png");
    BaseObject::LoadTexture(gUserData->woodTexture, "data/resources/textures/wood.png");

    // create depth texture
    glGenTextures(1, &gUserData->depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, gUserData->depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, DEPTH_MAP_FBO_WIDTH, DEPTH_MAP_FBO_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    for (int i = 0; i < CUBE_OBJECT_NUM; i++) {
        cube[i].setTexture(TEXTURE_STORED_COLOR, gUserData->container2Texture);
        cube[i].setTexture(TEXTURE_STORED_DEPTH, gUserData->depthMapTexture);
    }
    
    for (int i = 0; i < PLANE_OBJECT_NUM; i++) {
        plane[i].setTexture(TEXTURE_STORED_COLOR, gUserData->woodTexture);
        plane[i].setTexture(TEXTURE_STORED_DEPTH, gUserData->depthMapTexture);
    }

#ifdef DEPTH_MAP_VISUAL
    QuadObject *quad = gUserData->quadObject;
    quad->setTexture(TEXTURE_STORED_COLOR, gUserData->depthMapTexture);
#endif
}

static void initVertexData()
{
    CubeObject *cube = gUserData->cubeObject;
    PlaneObject *plane = gUserData->planeObject;

    GLuint cubeVAO, cubeVBO;
    GLuint planeVAO, planeVBO;
    unsigned int cubeVertexNum, planeVertexNum;

    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenVertexArrays(1, &planeVAO);
    glGenBuffers(1, &planeVBO);

    cubeVertexNum = CubeObject::createVertexObject(cube_vertices, sizeof(cube_vertices), cubeVAO, cubeVBO);
    planeVertexNum = PlaneObject::createVertexObject(plane_vertices, sizeof(plane_vertices), planeVAO, planeVBO);

    for (int i = 0; i < CUBE_OBJECT_NUM; i++) {
        cube[i].loadVertexData(cubeVAO, cubeVBO, cubeVertexNum);
    }

    for (int i = 0; i < PLANE_OBJECT_NUM; i++) {
        plane[i].loadVertexData(planeVAO, planeVBO, planeVertexNum);
    }

#ifdef DEPTH_MAP_VISUAL
    QuadObject *quad = gUserData->quadObject;

    GLuint quadVAO, quadVBO;
    unsigned int quadVertexNum;

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    quadVertexNum = QuadObject::createVertexObject(quad_vertices, sizeof(quad_vertices), quadVAO, quadVBO);
    quad->loadVertexData(quadVAO, quadVBO, quadVertexNum);
#endif
}

static void initSampler()
{
    gUserData->objectCommonShader->use();
    gUserData->objectCommonShader->setInt("diffuseSampler", TEXTURE_STORED_COLOR);
    gUserData->objectCommonShader->setInt("shadowMap", TEXTURE_STORED_DEPTH);

#ifdef DEPTH_MAP_VISUAL
    gUserData->quadShader->use();
    gUserData->quadShader->setInt("depthMap", TEXTURE_STORED_COLOR);
#endif
}

static void initDepthMapFBO()
{
    glGenFramebuffers(1, &gUserData->depthMapFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, gUserData->depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gUserData->depthMapTexture, 0);
    glDrawBuffers(0, GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

static void initUniform()
{
    glm::mat4 orthoProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(gUserData->lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 lightSpaceMatrix = orthoProjection * lightView;
    
    gUserData->objectCommonShader->use();
    gUserData->objectCommonShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    gUserData->objectCommonShader->setVec3("lightPos", gUserData->lightPos);
    gUserData->objectCommonShader->setVec3("viewPos", gUserData->camera.Position);

    gUserData->depthMapShader->use();
    gUserData->depthMapShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
}

int Init(ESContext *esContext)
{
    /* Init UserData struct */
    esContext->userData = malloc(sizeof(UserData));
    if (esContext->userData == nullptr) {
        LOG_FATAL("User Data malloc failed");
        return GL_FALSE;
    }
    memset(esContext->userData, 0, sizeof(UserData));
    gUserData = (UserData*)esContext->userData;

    gUserData->cubeObject = new CubeObject[CUBE_OBJECT_NUM];
    gUserData->planeObject = new PlaneObject[PLANE_OBJECT_NUM];
#ifdef DEPTH_MAP_VISUAL
    gUserData->quadObject = new QuadObject();
#endif

    /* Init Camera */
    gUserData->camera = Camera(glm::vec3(0.0f, 2.0f, 8.0f));

    /* Init light */
    gUserData->lightPos = glm::vec3(-2.0f, 4.0f, 2.0f);

    initShader();
    LOG_DEBUG("initShader finish");

    initVertexData();
    LOG_DEBUG("initVertexData finish");

    initObjectsMVP();
    LOG_DEBUG("initObjectsMVP finish");

    initTexture();
    LOG_DEBUG("initTexture finish");

    initSampler();
    LOG_DEBUG("initSampler finish");

    initUniform();
    LOG_DEBUG("initUniform finish");

    initDepthMapFBO();
    LOG_DEBUG("initDepthMapFBO finish");

    stbi_set_flip_vertically_on_load(true);
    gModel = new Model("data/backpack/backpack.obj");

    /* Set the viewport */
    glViewport(0, 0, esContext->width, esContext->height);
    glEnable(GL_DEPTH_TEST);

    return GL_TRUE;
}

static void updateUniform(float deltaTime)
{
    glm::mat4 detaAngleMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(deltaTime*60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    gUserData->lightPos = glm::mat3(detaAngleMatrix) * gUserData->lightPos;

    glm::mat4 orthoProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(gUserData->lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    glm::mat4 lightSpaceMatrix = orthoProjection * lightView;
    
    gUserData->objectCommonShader->use();
    gUserData->objectCommonShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
    gUserData->objectCommonShader->setVec3("lightPos", gUserData->lightPos);
    gUserData->objectCommonShader->setVec3("viewPos", gUserData->camera.Position);

    gUserData->depthMapShader->use();
    gUserData->depthMapShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);
}

void Update(ESContext *esContext, float deltaTime)
{
    // LOG_DEBUG("update deta time: %f", deltaTime);  // 0.0x
    // updateUniform(deltaTime);

    CubeObject *cube = gUserData->cubeObject;
    PlaneObject *plane = gUserData->planeObject;

    glm::mat4 camPerspProjection = glm::perspective(glm::radians(gUserData->camera.Zoom), (float)esContext->width / (float)esContext->height, 0.1f, 100.0f);
    glm::mat4 camView = gUserData->camera.GetViewMatrix();

    for (int i = 0; i < CUBE_OBJECT_NUM; i++) {
        cube[i].setViewMatrix(camView);
        cube[i].setProjectionMatrix(camPerspProjection);
    }

    for (int i = 0; i < PLANE_OBJECT_NUM; i++) {
        plane[i].setViewMatrix(camView);
        plane[i].setProjectionMatrix(camPerspProjection);
    }
}

static void renderCube(Shader* pShader)
{
    LOG_DEBUG("renderCube start");

    CubeObject *cube = gUserData->cubeObject;

    for (int i = 0; i < CUBE_OBJECT_NUM; i++)
    {
        cube[i].setShader(pShader);
        cube[i].updateMVP();
        // cube[i].active2DTexture(TEXTURE_STORED_COLOR);
        // cube[i].updateModelMatrix();
        cube[i].startRender();
    }

    LOG_DEBUG("renderCube end");
}

static void renderPlane(Shader* pShader)
{
    LOG_DEBUG("renderPlane start");

    PlaneObject *plane = gUserData->planeObject;

    for (int i = 0; i < PLANE_OBJECT_NUM; i++)
    {
        plane[i].setShader(pShader);
        plane[i].updateMVP();
        // plane[i].active2DTexture(TEXTURE_STORED_COLOR);
        // plane[i].updateModelMatrix();
        plane[i].startRender();
    }
    LOG_DEBUG("renderPlane end");
}

#ifdef DEPTH_MAP_VISUAL
static void renderQuad(Shader* pShader)
{
    LOG_DEBUG("renderQuad start");

    QuadObject *quad = gUserData->quadObject;
    quad->setShader(pShader);
    quad->updateMVP();
    quad->active2DTexture(TEXTURE_STORED_COLOR);  // default
    quad->startRender();

    LOG_DEBUG("renderQuad end");
}
#endif

void Draw(ESContext *esContext)
{
    LOG_DEBUG("draw start");

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

#ifdef DEPTH_MAP_VISUAL
    glViewport(0, 0, DEPTH_MAP_FBO_WIDTH, DEPTH_MAP_FBO_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, gUserData->depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    renderCube(gUserData->depthMapShader);
    renderPlane(gUserData->depthMapShader);
    initModelMVP(gUserData->depthMapShader);
    gModel->Draw(*gUserData->depthMapShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // reset viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderQuad(gUserData->quadShader);

#else
    CubeObject *cube = gUserData->cubeObject;
    PlaneObject *plane = gUserData->planeObject;

    glViewport(0, 0, DEPTH_MAP_FBO_WIDTH, DEPTH_MAP_FBO_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, gUserData->depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
    renderPlane(gUserData->depthMapShader);
    renderCube(gUserData->depthMapShader);
    initModelMVP(gUserData->depthMapShader);
    gModel->Draw(*gUserData->depthMapShader);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (int i = 0; i < PLANE_OBJECT_NUM; i++) {
        plane[i].setShader(gUserData->objectCommonShader);
        plane[i].updateMVP();
        plane[i].active2DTexture(TEXTURE_STORED_COLOR);
        plane[i].active2DTexture(TEXTURE_STORED_DEPTH);
        plane[i].startRender();
    }

    for (int i = 0; i < CUBE_OBJECT_NUM; i++) {
        cube[i].setShader(gUserData->objectCommonShader);
        cube[i].updateMVP();
        cube[i].active2DTexture(TEXTURE_STORED_COLOR);
        cube[i].active2DTexture(TEXTURE_STORED_DEPTH);
        cube[i].startRender();
    }

    initModelMVP(gUserData->objectCommonShader);
    gModel->Draw(*gUserData->objectCommonShader);
#endif

    LOG_DEBUG("draw end");
}

void Shutdown(ESContext *esContext)
{
    if (gUserData->woodTexture) {
        glDeleteTextures(1, &gUserData->woodTexture);
    }
    
    if (gUserData->container2Texture) {
        glDeleteTextures(1, &gUserData->container2Texture);
    }

    if (gUserData->depthMapTexture) {
        glDeleteTextures(1, &gUserData->depthMapTexture);
    }

    if (gUserData->depthMapFBO) {
        glDeleteFramebuffers(1, &gUserData->depthMapFBO);
    }

    if (gUserData->cubeObject) {
        delete[] gUserData->cubeObject;
        gUserData->cubeObject = nullptr;
    }

    if (gUserData->planeObject) {
        delete[] gUserData->planeObject;
        gUserData->planeObject = nullptr;
    }

    if (gUserData->objectCommonShader) {
        delete gUserData->objectCommonShader;
        gUserData->objectCommonShader = nullptr;
    }

    if (gUserData->depthMapShader) {
        delete gUserData->depthMapShader;
        gUserData->depthMapShader = nullptr;
    }

#ifdef DEPTH_MAP_VISUAL
    if (gUserData->quadObject) {
        delete gUserData->quadObject;
        gUserData->quadObject = nullptr;
    }

    if (gUserData->quadShader) {
        delete gUserData->quadShader;
        gUserData->quadShader = nullptr;
    }
#endif
}

extern "C" int esMain(ESContext *esContext)
{
    esCreateWindow(esContext, "Shadw Mapping", SCR_WIDTH, SCR_HEIGHT, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

    if (!Init(esContext))
    {
        LOG_FATAL("esContext init failed!");
        return GL_FALSE;
    }
    LOG_DEBUG("esContext init success");

    esRegisterShutdownFunc(esContext, Shutdown);
#ifndef DEPTH_MAP_VISUAL
    esRegisterUpdateFunc(esContext, Update);
#endif
    esRegisterDrawFunc(esContext, Draw);
    // esRegisterKeyFunc(esContext, KeyEvent);
    esRegisterMouseFunc(esContext, MouseMoveEvent, MouseWheelEvent);

    return GL_TRUE;
}
