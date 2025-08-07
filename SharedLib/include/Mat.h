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
    __m128 row0 = pMat->m[0].data;
    __m128 row1 = pMat->m[1].data;
    __m128 row2 = pMat->m[2].data;
    __m128 row3 = pMat->m[3].data;

    // Step 1: Unpack pairs of rows
    __m128 tmp0 = _mm_unpacklo_ps(row0, row1); // [x0 x1 y0 y1]
    __m128 tmp1 = _mm_unpackhi_ps(row0, row1); // [z0 z1 w0 w1]
    __m128 tmp2 = _mm_unpacklo_ps(row2, row3); // [x2 x3 y2 y3]
    __m128 tmp3 = _mm_unpackhi_ps(row2, row3); // [z2 z3 w2 w3]

    Mat4 result;

    // Final assembly
    result.m[0].data = _mm_movelh_ps(tmp0, tmp2); // [x0 x1 x2 x3]
    result.m[1].data = _mm_movehl_ps(tmp2, tmp0); // [y0 y1 y2 y3]
    result.m[2].data = _mm_movelh_ps(tmp1, tmp3); // [z0 z1 z2 z3]
    result.m[3].data = _mm_movehl_ps(tmp3, tmp1); // [w0 w1 w2 w3]

    return result;
}

inline Mat4 Mat4_MultiplyMat4(const Mat4* pMatA, const Mat4* pMatB)
{
    const Mat4 transposedB = Mat4_Transpose(pMatB);

    Mat4 result;
    for (i32 i = 0; i < 4; ++i)
    {
        result.m[i].data = _mm_set_ps(
            _mm_cvtss_f32(_mm_dp_ps(pMatA->m[i].data, transposedB.m[3].data, 0b11110001)),
            _mm_cvtss_f32(_mm_dp_ps(pMatA->m[i].data, transposedB.m[2].data, 0b11110001)),
            _mm_cvtss_f32(_mm_dp_ps(pMatA->m[i].data, transposedB.m[1].data, 0b11110001)),
            _mm_cvtss_f32(_mm_dp_ps(pMatA->m[i].data, transposedB.m[0].data, 0b11110001)));
    }

    return result;
}

inline Vec4 Mat4_MultiplyVec4(const Mat4* pMat, const Vec4* pVec)
{
    return Vec4_Create(
        _mm_cvtss_f32(_mm_dp_ps(pMat->m[0].data, pVec->data, 0b11110001)),
        _mm_cvtss_f32(_mm_dp_ps(pMat->m[1].data, pVec->data, 0b11110001)),
        _mm_cvtss_f32(_mm_dp_ps(pMat->m[2].data, pVec->data, 0b11110001)),
        _mm_cvtss_f32(_mm_dp_ps(pMat->m[3].data, pVec->data, 0b11110001)));
}

inline Mat4 Mat4_Translate(const Vec3* pVec)
{
    Mat4 result;
    result.m[0] = Vec4_Create(1.0f, 0.0f, 0.0f, 0.0f);
    result.m[1] = Vec4_Create(0.0f, 1.0f, 0.0f, 0.0f);
    result.m[2] = Vec4_Create(0.0f, 0.0f, 1.0f, 0.0f);
    result.m[3] = Vec4_Create(pVec->x, pVec->y, pVec->z, 1.0f);
    return result;
}

inline Mat4 Mat4_Scale(const Vec3* pVec)
{
    Mat4 result;
    result.m[0] = Vec4_Create(pVec->x, 0.0f, 0.0f, 0.0f);
    result.m[1] = Vec4_Create(0.0f, pVec->y, 0.0f, 0.0f);
    result.m[2] = Vec4_Create(0.0f, 0.0f, pVec->z, 0.0f);
    result.m[3] = Vec4_Create(0.0f, 0.0f, 0.0f, 1.0f);
    return result;
}

inline Mat4 Mat4_ScaleUniform(f32 scale)
{
    Vec3 scaleVec = {scale, scale, scale};
    return Mat4_Scale(&scaleVec);
}
