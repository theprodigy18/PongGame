#pragma once

#include "Vec.h"

typedef struct _MeshBuffer
{
    GLuint vao;
    GLuint vbo;
    GLuint ibo;
} _MeshBuffer;

typedef _MeshBuffer* MeshBuffer;

typedef struct _Vertex
{
    Vec3 pos;
    Vec3 color;
} Vertex;

bool DROP_CreateMeshBuffer(const Vertex* pVertices, const GLint* pIndices, size_t vertexCount, size_t indexCount, MeshBuffer* pMeshBuffer);
bool DROP_CreateMeshBufferWithSharedVBO(const GLuint vbo, const GLint* pIndices, size_t indexCount, MeshBuffer* pMeshBuffer);
void DROP_DestroyMeshBuffer(MeshBuffer* pMeshBuffer, bool onlyVAO);
