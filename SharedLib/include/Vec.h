#pragma once

typedef struct _Vec2
{
	union
	{
        struct { f32 x, y; };
		f32 v[2];
	};
} Vec2;

typedef struct _Vec3
{
	union
	{
		struct { f32 x, y, z; };
		f32 v[3];
	};
} Vec3;

typedef __declspec(align(16)) struct _Vec4
{
	union
	{
		f32 v[4];
		struct { f32 x, y, z, w; };
		__m128 data;
	};
} Vec4;


inline Vec4 Vec4_Create(f32 x, f32 y, f32 z, f32 w)
{
    return (Vec4){.data = _mm_set_ps(w, z, y, x)};
}