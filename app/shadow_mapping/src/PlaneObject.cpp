#include "object.h"
#include "log.h"


PlaneObject::PlaneObject(const char *vs, const char *fs): BaseObject(vs, fs)
{

};

PlaneObject::PlaneObject() {

};

PlaneObject::~PlaneObject() {

};

void PlaneObject::startRender()
{
    GLuint vao = mVAO[VAO_VERTEX_DATA_INDEX];

    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, this->getVertexNum());
    glBindVertexArray(0);
}

unsigned int PlaneObject::createVertexObject(const float vertexs[], unsigned int size, GLuint vao, GLuint vbo)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, size, vertexs, GL_STATIC_DRAW);

    glBindVertexArray(vao);
    glEnableVertexAttribArray(PLANE_VERTEX_POS_INDX);
    glVertexAttribPointer(PLANE_VERTEX_POS_INDX, PLANE_VERTEX_POS_SIZE, GL_FLOAT, GL_FALSE, PLANE_VERTEX_STRIDE, (void *)0);
    glEnableVertexAttribArray(PLANE_VERTEX_NORMAL_INDX);
    glVertexAttribPointer(PLANE_VERTEX_NORMAL_INDX, PLANE_VERTEX_NORMAL_SIZE, GL_FLOAT, GL_FALSE, PLANE_VERTEX_STRIDE, (void *)(0 + PLANE_VERTEX_POS_SIZE*sizeof(float)));
    glEnableVertexAttribArray(PLANE_VERTEX_TEXTURE_INDX);
    glVertexAttribPointer(PLANE_VERTEX_TEXTURE_INDX, PLANE_VERTEX_TEXTURE_SIZE, GL_FLOAT, GL_FALSE, PLANE_VERTEX_STRIDE, (void *)(0 + (PLANE_VERTEX_POS_SIZE + PLANE_VERTEX_NORMAL_SIZE)*sizeof(float)));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return PLANE_VERTEX_NUM(size);
}
