#ifndef __OBJECT_H_
#define __OBJECT_H_

#include "OpenGL/shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

class BaseObject {
public:
    BaseObject();
    BaseObject(Shader* shader);
    BaseObject(const char *vs, const char *fs);
    virtual ~BaseObject();
    void setShader(Shader* shader);
    void loadVertexData(GLuint vao, GLuint vbo, unsigned int vertexNum);
    virtual void startRender() = 0;
    virtual void loadTexture(const unsigned int textureIndex, char const *path);
    void setTexture(const unsigned int index, const GLuint textureID);
    GLuint getTexture(const unsigned int index);
    void active2DTexture(const unsigned int index);
    void bindSamplerAndTexture(const std::string &samplerName, const int enumTextureIndex);
    Shader* getShaderPtr();
    GLuint getVAO(const unsigned int index);
    GLuint getVBO(const unsigned int index);
    unsigned int getVAONum();
    unsigned int getVBONum();
    void setVAO(const unsigned int index, const GLuint vao);
    void setVBO(const unsigned int index, const GLuint vbo);
    GLuint getVertexNum();

    void setMVP(const glm::mat4 &model, \
                 const glm::mat4 &view, \
                 const glm::mat4 &projection);
    
    void setModelMatrix(const glm::mat4 &model);
    void setViewMatrix(const glm::mat4 &view);
    void setProjectionMatrix(const glm::mat4 &projection);
    void updateModelMatrix();
    void updateViewMatrix();
    void updateProjectionMatrix();
    void updateMVP();
    void showModelMatrix();
    void showViewMatrix();
    void showProjectionMatrix();
    void setVertexNum(const unsigned int num);

    void updateUniformVec3(const std::string &name, const glm::vec3 &value) const;
    void updateUniformMat4(const std::string &name, const glm::mat4 &mat) const;

    static void LoadTexture(const GLuint &textureID, char const *path);

protected:
    Shader* mShader;
    GLuint* mVAO;
    GLuint mVAONum;
    GLuint* mVBO;
    GLuint mVBONum;
    GLuint* mTexture;
    GLuint mTextureNum;
    GLuint mVertexNum;
    glm::mat4 mModelMatrix;
    glm::mat4 mViewMatrix;
    glm::mat4 mProjectionMatrix;

private:
    void showMat4(const glm::mat4 &mat);
};


class CubeObject: public BaseObject {
public:
    CubeObject();
    CubeObject(const char *vs, const char *fs);
    ~CubeObject();

    void startRender();

    static unsigned int createVertexObject(const float vertexs[], unsigned int size, GLuint vao, GLuint vbo);
};


class PlaneObject: public BaseObject {
public:
    PlaneObject();
    PlaneObject(const char *vs, const char *fs);
    ~PlaneObject();

    void startRender();

    static unsigned int createVertexObject(const float vertexs[], unsigned int size, GLuint vao, GLuint vbo);
};


class QuadObject: public BaseObject {
public:
    QuadObject();
    ~QuadObject();

    void startRender();

    static unsigned int createVertexObject(const float vertexs[], unsigned int size, GLuint vao, GLuint vbo);
};

#endif
