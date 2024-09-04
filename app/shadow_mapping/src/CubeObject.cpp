#include "object.h"
#include "log.h"


CubeObject::CubeObject(const char *vs, const char *fs): BaseObject(vs, fs)
{

};

CubeObject::CubeObject() {

};

CubeObject::~CubeObject() {

};

unsigned int CubeObject::createVertexObject(const float vertexs[], unsigned int size, GLuint vao, GLuint vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertexs, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(CUBE_VERTEX_POS_INDX);
    glVertexAttribPointer(CUBE_VERTEX_POS_INDX, CUBE_VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, CUBE_VERTEX_STRIDE, (void *)0);
    glEnableVertexAttribArray(CUBE_VERTEX_NORMAL_INDX);
    glVertexAttribPointer(CUBE_VERTEX_NORMAL_INDX, CUBE_VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, CUBE_VERTEX_STRIDE, (void *)(0 + CUBE_VERTEX_POS_SIZE * sizeof(float)));
    glEnableVertexAttribArray(CUBE_VERTEX_TEXTURE_INDX);
    glVertexAttribPointer(CUBE_VERTEX_TEXTURE_INDX, CUBE_VERTEX_TEXTURE_SIZE, GL_FLOAT, GL_FALSE, CUBE_VERTEX_STRIDE, (void *)(0 + (CUBE_VERTEX_POS_SIZE + CUBE_VERTEX_NORMAL_SIZE) * sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return CUBE_VERTEX_NUM(size);
}

void CubeObject::startRender()
{
    GLuint vao = mVAO[VAO_VERTEX_DATA_INDEX];
    
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, this->getVertexNum());
    glBindVertexArray(0);
}
