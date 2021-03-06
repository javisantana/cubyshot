#pragma once
#ifndef __MATH__H__
#define __MATH__H__

#include <math.h>

#define INLINE __inline

#define PI 3.141516f

#define RAD2DEG (180.0f/PI)
#define DEG2RAD (PI/180.0f)
#undef max
#undef min
// seed can be changed and set to magic value!
extern unsigned int seed;

float max(float a,float b);
float min(float a,float b);
float linear(float a,float b, float t);
float smoothstep(float a, float b,float t);
float step(float a,float t);
float pulse(float a,float b,float t);
float clamp(float a,float b,float t);
float _frac(float a);
float mod(float a,float t);
float sfrand( unsigned int *seed );
float randf( void );
float rand01( void );
///return random number between 0 and i - 1
int randi(int i);

float perlin2d(float x, float y);
float fbm(float x, float y);

//vector

typedef float vec3f[3];
#define VMOV3(o, x, y, z) o[0] = x; o[1] = y; o[2] = z
#define VMOV(o, a) o[0] = a[0];o[1] = a[1];o[2] = a[2]
#define VADD(o, a, b) o[0] = a[0] + b[0];o[1] = a[1] + b[1];o[2] = a[2] + b[2]
#define VSUB(o, a, b) o[0] = a[0] - b[0];o[1] = a[1] - b[1];o[2] = a[2] - b[2]
#define MADD(o, a, t, b) o[0] = a[0] + t*b[0];o[1] = a[1] + t*b[1];o[2] = a[2] + t*b[2]
#define VMUL(o, a, t) o[0] = a[0]*t; o[1] = a[1]*t; o[2] = a[2]*t
#define VDIST(a) sqrtf(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);


int in_range(float x, float var, float p);
bool is_inside_2d(vec3f pos, vec3f margin, vec3f p);

#endif
