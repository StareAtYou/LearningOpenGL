#include "object.h"
#include "log.h"


QuadObject::QuadObject() {

};

QuadObject::~QuadObject() {

};

void QuadObject::startRender()
{
    GLuint vao = mVAO[VAO_VERTEX_DATA_INDEX];

    mShader->use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, this->getVertexNum());
    glBindVertexArray(0);
}

unsigned int QuadObject::createVertexObject(const float vertexs[], unsigned int size, GLuint vao, GLuint vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertexs, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(QUAD_VERTEX_POS_INDX);
    glVertexAttribPointer(QUAD_VERTEX_POS_INDX, QUAD_VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, QUAD_VERTEX_STRIDE, (void *)0);
    glEnableVertexAttribArray(QUAD_VERTEX_TEXTURE_INDX);
    glVertexAttribPointer(QUAD_VERTEX_TEXTURE_INDX, QUAD_VERTEX_TEXTURE_SIZE, GL_FLOAT, GL_FALSE, QUAD_VERTEX_STRIDE, (void *)(0 + (QUAD_VERTEX_POS_SIZE)*sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return QUAD_VERTEX_NUM(size);
}
