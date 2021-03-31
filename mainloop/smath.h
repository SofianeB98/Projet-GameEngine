#pragma once
#include <nmmintrin.h>
#include <immintrin.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

union float4
{
	__m128 a4; float a[4];

};



inline float4 Add(float4 const a, float4 const b)
{
	return { _mm_add_ps(a.a4, b.a4) };

}

inline float4 Sub(float4 const a, float4 const b)
{
	return { _mm_sub_ps(a.a4, b.a4) };
}

inline float4 operator+(float4 a, float4 b)
{
	return { _mm_add_ps(a.a4, b.a4) };
}




