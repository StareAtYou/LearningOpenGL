#include "object.h"
#include "log.h"

#define STB_IMAGE_IMPLEMENTATION
#include "Utils/stb_image.h"


BaseObject::BaseObject()
{
    this->mShader = nullptr;
    this->mVAO = 0;
    this->mVBO = 0;
    this->mVertexNum = 0;
    this->mVAONum = 1;
    this->mVBONum = 1;
    this->mTexture = 0;
    this->mTextureNum = 2;

    this->mModelMatrix = glm::mat4();
    this->mViewMatrix = glm::mat4();
    this->mProjectionMatrix = glm::mat4();

    this->mVAO = new GLuint[mVAONum];
    this->mVBO = new GLuint[mVBONum];
    this->mTexture = new GLuint[mTextureNum];
}

BaseObject::BaseObject(Shader* shader)
{
    this->mShader = mShader;
    this->mVAO = 0;
    this->mVBO = 0;
    this->mVertexNum = 0;
    this->mVAONum = 1;
    this->mVBONum = 1;
    this->mTexture = 0;
    this->mTextureNum = 2;

    this->mModelMatrix = glm::mat4();
    this->mViewMatrix = glm::mat4();
    this->mProjectionMatrix = glm::mat4();

    this->mVAO = new GLuint[mVAONum];
    this->mVBO = new GLuint[mVBONum];
    this->mTexture = new GLuint[mTextureNum];
}

BaseObject::BaseObject(const char *vs, const char *fs)
{
    this->mShader = new Shader(vs, fs);
    if (this->mShader == nullptr) {
        LOG_FATAL("Shader init failed!");
        return;
    }

    this->mVAO = 0;
    this->mVBO = 0;
    this->mVertexNum = 0;
    this->mVAONum = 1;
    this->mVBONum = 1;
    this->mTexture = 0;
    this->mTextureNum = 2;

    this->mModelMatrix = glm::mat4();
    this->mViewMatrix = glm::mat4();
    this->mProjectionMatrix = glm::mat4();

    this->mVAO = new GLuint[mVAONum];
    this->mVBO = new GLuint[mVBONum];
    this->mTexture = new GLuint[mTextureNum];
}

BaseObject::~BaseObject()
{
    if (this->mShader) {
        delete this->mShader;
        this->mShader = nullptr;
    }

    for (int i=0; i<mVAONum; i++) {
        if (this->mVAO[i]) {
            glDeleteVertexArrays(1, &this->mVAO[i]);
        }
    }
    delete[] this->mVAO;

    for (int i=0; i<mVBONum; i++) {
        if (this->mVBO[i]) {
            glDeleteBuffers(1, &this->mVBO[i]);
        }
    }
    delete[] this->mVBO;

    for (int i=0; i<mTextureNum; i++) {
        if (this->mTexture[i]) {
            glDeleteTextures(1, &this->mTexture[i]);
        }
    }
    delete[] this->mTexture;

    this->mVertexNum = 0;
    this->mTextureNum = 0;
    this->mModelMatrix = glm::mat4();
    this->mViewMatrix = glm::mat4();
    this->mProjectionMatrix = glm::mat4();
}

void BaseObject::setShader(Shader* shader)
{
    // /* memory leak */
    // if (this->mShader) {
    //     delete this->mShader;
    // }

    this->mShader = shader;
}

void BaseObject::showMat4(const glm::mat4 &mat)
{
    int rows = mat.length();
    int cols = mat[0].length();

    for (int i=0; i<rows; i++) {
        for (int j=0; j<cols; j++) {
            std::cout << mat[j][i] << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void BaseObject::showModelMatrix()
{
    showMat4(mModelMatrix);
}

void BaseObject::showViewMatrix()
{
    showMat4(mViewMatrix);
}

void BaseObject::showProjectionMatrix()
{
    showMat4(mProjectionMatrix);
}

GLuint BaseObject::getVAO(unsigned int index)
{
    if (index >= this->mVAONum) {
        LOG_FATAL("Index out of range!!!");
        return 0;
    }

    return this->mVAO[index];
}

GLuint BaseObject::getVBO(unsigned int index)
{
    if (index >= this->mVBONum) {
        LOG_FATAL("Index out of range!!!");
        return 0;
    }

    return this->mVBO[index];
}

unsigned int BaseObject::getVAONum()
{
    return this->mVAONum;
}

unsigned int BaseObject::getVBONum()
{
    return this->mVBONum;
}

void BaseObject::setVAO(const unsigned int index, const GLuint vao)
{
    if (index >= this->mVAONum) {
        LOG_FATAL("Index out of range!!!");
        return;
    }

    this->mVAO[index] = vao;
}

void BaseObject::setVBO(const unsigned int index, const GLuint vbo)
{
    if (index >= this->mVBONum) {
        LOG_FATAL("Index out of range!!!");
        return;
    }

    this->mVBO[index] = vbo;
}

void BaseObject::setVertexNum(unsigned int num)
{
    this->mVertexNum = num;
}

GLuint BaseObject::getVertexNum()
{
    return this->mVertexNum;
}

void BaseObject::loadVertexData(GLuint vao, GLuint vbo, unsigned int vertexNum)
{
    this->setVAO(VAO_VERTEX_DATA_INDEX, vao);
    this->setVBO(VAO_VERTEX_DATA_INDEX, vbo);
    this->setVertexNum(vertexNum);
    LOG_INFO("Vertex data stored, vertex num is %d", getVertexNum());
}

void BaseObject::LoadTexture(const GLuint &textureID, char const *path)
{
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

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        LOG_FATAL("Texture failed to load at path: %s", path);
        stbi_image_free(data);
    }
}

void BaseObject::loadTexture(unsigned int index, char const *path)
{
    if (index >= this->mTextureNum) {
        LOG_FATAL("Index out of range!!!");
        return;
    }

    LoadTexture(this->mTexture[index], path);
}

void BaseObject::setTexture(unsigned int index, GLuint textureID)
{
    if (index >= this->mTextureNum) {
        LOG_FATAL("Index out of range!!!");
        return;
    }

    this->mTexture[index] = textureID;
}

GLuint BaseObject::getTexture(unsigned int index)
{
    if (index >= this->mTextureNum) {
        LOG_FATAL("Index out of range!!!");
        return 0;
    }

    return this->mTexture[index];
}

void BaseObject::active2DTexture(unsigned int index)
{
    if (index >= this->mTextureNum) {
        LOG_FATAL("Index out of range!!!");
        return;
    }

    this->mShader->use();
    glActiveTexture(GL_TEXTURE0 + index);
    glBindTexture(GL_TEXTURE_2D, this->mTexture[index]);
}

void BaseObject::bindSamplerAndTexture(const std::string &samplerName, const int enumTextureIndex)
{
    if (this->mShader == nullptr) {
        LOG_FATAL("Init shader first");
        return;
    }

    this->mShader->use();
    this->mShader->setInt(samplerName, enumTextureIndex);
}

void BaseObject::setMVP(const glm::mat4 &model, \
                        const glm::mat4 &view, \
                        const glm::mat4 &projection)
{
    this->mModelMatrix = model;
    this->mViewMatrix = view;
    this->mProjectionMatrix = projection;
}

void BaseObject::setModelMatrix(const glm::mat4 &model)
{
    this->mModelMatrix = model;
}

void BaseObject::setViewMatrix(const glm::mat4 &view)
{
    this->mViewMatrix = view;
}

void BaseObject::setProjectionMatrix(const glm::mat4 &projection)
{
    this->mProjectionMatrix = projection;
}

void BaseObject::updateModelMatrix()
{
    if (this->mShader == nullptr) {
        LOG_FATAL("Init shader first");
        return;
    }

    this->mShader->use();
    this->mShader->setMat4("model", this->mModelMatrix);
}

void BaseObject::updateViewMatrix()
{
    if (this->mShader == nullptr) {
        LOG_FATAL("Init shader first");
        return;
    }

    this->mShader->use();
    this->mShader->setMat4("view", this->mViewMatrix);
}

void BaseObject::updateProjectionMatrix()
{
    if (this->mShader == nullptr) {
        LOG_FATAL("Init shader first");
        return;
    }

    this->mShader->use();
    this->mShader->setMat4("projection", this->mProjectionMatrix);
}

void BaseObject::updateMVP()
{
    if (this->mShader == nullptr) {
        LOG_FATAL("Init shader first");
        return;
    }

    this->mShader->use();
    this->mShader->setMat4("model", this->mModelMatrix);
    this->mShader->setMat4("view", this->mViewMatrix);
    this->mShader->setMat4("projection", this->mProjectionMatrix);
}

void BaseObject::updateUniformVec3(const std::string &name, const glm::vec3 &value) const
{
    this->mShader->use();
    this->mShader->setVec3(name, value);
}

void BaseObject::updateUniformMat4(const std::string &name, const glm::mat4 &mat) const
{
    this->mShader->use();
    this->mShader->setMat4(name, mat);
}
