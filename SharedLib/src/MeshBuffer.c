#include "pch.h"
#include "MeshBuffer.h"
#include "Graphics.h"

bool DROP_CreateMeshBuffer(const Vertex* pVertices, const GLint* pIndices, size_t vertexCount, size_t indexCount, MeshBuffer* pMeshBuffer)
{
    ASSERT_MSG(pVertices && pIndices, "Vertex and index buffers are null.");
    ASSERT_MSG(vertexCount && indexCount, "Vertex and index counts are zero.");
    *pMeshBuffer = NULL;

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(Vertex), pVertices, GL_STATIC_DRAW);

    GLuint ibo = 0;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLint), pIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, color));

    glBindVertexArray(0);

    MeshBuffer meshBuffer = ALLOC_SINGLE(_MeshBuffer);
    ASSERT(meshBuffer);
    meshBuffer->vao = vao;
    meshBuffer->vbo = vbo;
    meshBuffer->ibo = ibo;

    *pMeshBuffer = meshBuffer;

    return true;
}

bool DROP_CreateMeshBufferWithSharedVBO(const GLuint vbo, const GLint* pIndices, size_t indexCount, MeshBuffer* pMeshBuffer)
{
	ASSERT_MSG(vbo, "VBO are zero.");
    ASSERT_MSG(pIndices, "Index buffer are null.");
    ASSERT_MSG(indexCount, "Index count are zero.");
    *pMeshBuffer = NULL;

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    GLuint ibo = 0;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLint), pIndices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, pos));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*) offsetof(Vertex, color));

    glBindVertexArray(0);

    MeshBuffer meshBuffer = ALLOC_SINGLE(_MeshBuffer);
    ASSERT(meshBuffer);
    meshBuffer->vao = vao;
    meshBuffer->vbo = vbo;
    meshBuffer->ibo = ibo;

    *pMeshBuffer = meshBuffer;

    return true;
}

void DROP_DestroyMeshBuffer(MeshBuffer* pMeshBuffer, bool onlyVAO)
{
    ASSERT_MSG(pMeshBuffer && *pMeshBuffer, "Mesh buffer is null.");
    MeshBuffer meshBuffer = *pMeshBuffer;

    if (meshBuffer)
    {
        glBindVertexArray(0);
        glDeleteVertexArrays(1, &meshBuffer->vao);
        glDeleteBuffers(1, &meshBuffer->ibo);

        if (!onlyVAO)
        {
            glDeleteBuffers(1, &meshBuffer->vbo);
        }

        meshBuffer->vao = 0;
        meshBuffer->vbo = 0;
        meshBuffer->ibo = 0;

        FREE(meshBuffer);
    }

    *pMeshBuffer = NULL;
}