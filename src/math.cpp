#include "math.h"

float max(float a,float b) 
{
  return a > b ? a: b;
}

float min(float a,float b) 
{
  return a < b ? a: b;
}

float linear(float a,float b, float t)
{
	return a+t*(b-a);
}

float smoothstep(float a, float b,float t)
{
	if(t < a) 
		return 0.0f;
	if(t > b) 
		return 1.0f;
	t = (t-a)/(b-a);
	return (t) * (t) * (3.0f - 2.0f*(t)) ;
}

float step(float a,float t)
{
	if(t <= a) return 0.0f;
	return 1.0f;
}

float pulse(float a,float b,float t)
{
	return step(a,t) - step(b,t);
}

float clamp(float a,float b,float t)
{
	return min(max(t,a),b);
}

float _frac(float a)
{
	return a - (int)a;//(int)(a-0.5);
}

float mod(float a,float t)
{
	return _frac(t/a);
}

//internal seed
unsigned int seed = 1;
float rand01()
{
  return 0.5f*(sfrand(&seed) + 1.0f);
}

//from iq
float sfrand(unsigned int *seed )
{
    float res;
    seed[0] *= 16807;
    *((unsigned int *) &res) = ( ((unsigned int)seed[0])>>9 ) | 0x40000000;
    return( res-3.0f );
}

float randf( void )
{
    return sfrand(&seed);
}

int randi(int i)
{
	seed = (seed*214013+2531011);
	return (seed>>16) % i;
}

float slerp(float a, float b, float x)
{     
    return a+(b-a)*x*x*(3-2*x);                
}

float noisei(register int x)
{
	x*=seed;
    x = (x<<13)^x;
    return (((x * (x * x * 15731 + 789221) + 1376312589) & 0x7fffffff) / 2147483648.0f);
}
float perlin2d(float x, float y)
{
	int	xi = (int)x;
	float xf = _frac(x);
	int	yi = (int)y;
	float yf = _frac(y);	

	return slerp( slerp(noisei((yi << 8) + xi), noisei((yi << 8) + xi + 1) , xf),
                  slerp(noisei(((yi + 1) << 8)  + xi), noisei(((yi + 1)<<8) + xi + 1) , xf), 
				  yf);
	
}

float fbm(float x, float y)
{
  int i;
  float step = 1.0f;
  float amp = 0.5f;
  float res = 0.0f;

  for(i = 0; i < 4; ++i)
  {
    res += amp*perlin2d(x*step, y*step);
    amp *= 0.5f;
    step *= 2.0f;
  }
  return res;
}

int in_range(float x, float var, float p)
{
	var *= 0.5f;
	return p <= (x + var) && 
			p >= (x - var);
}

bool is_inside_2d(vec3f pos, vec3f margin, vec3f p)
{
	return in_range(pos[0], margin[0], p[0]) &&
			in_range(pos[1], margin[1], p[1]);
}