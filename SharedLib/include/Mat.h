#pragma once

#include "Vec.h"

typedef __declspec(align(16)) struct _Mat4
{
    Vec4 m[4];
} Mat4;

inline Mat4 Mat4_Identity()
{
    Mat4 result;
    result.m[0] = Vec4_Create(1.0f, 0.0f, 0.0f, 0.0f);
    result.m[1] = Vec4_Create(0.0f, 1.0f, 0.0f, 0.0f);
    result.m[2] = Vec4_Create(0.0f, 0.0f, 1.0f, 0.0f);
    result.m[3] = Vec4_Create(0.0f, 0.0f, 0.0f, 1.0f);
    return result;
}

inline Mat4 Mat4_Transpose(const Mat4* pMat)
{
    Mat4 result;
	result.m[0] = Vec4_Create(pMat->m[0].x, pMat->m[1].x, pMat->m[2].x, pMat->m[3].x);
	result.m[1] = Vec4_Create(pMat->m[0].y, pMat->m[1].y, pMat->m[2].y, pMat->m[3].y);
	result.m[2] = Vec4_Create(pMat->m[0].z, pMat->m[1].z, pMat->m[2].z, pMat->m[3].z);
	result.m[3] = Vec4_Create(pMat->m[0].w, pMat->m[1].w, pMat->m[2].w, pMat->m[3].w);
	return result;
}