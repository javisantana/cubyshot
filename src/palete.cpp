#include "renderables.h"

#define _RGB(r,g,b) r.0f/255.0f, g.0f/255.0f, b.0f/255.0f

float blue[5][3] = {
{_RGB(11, 44, 51)},
{_RGB(33, 83, 84)},
{_RGB(90, 156, 154)},
{_RGB(136, 224, 225)},
{_RGB(180, 237, 230)}
};

float* pal_color(int i)
{
	return blue[i];
}

void set_pal_color(int i, float alpha)
{
	
	float* c = pal_color(i);
	glColor4f(c[0], c[1], c[2], alpha);
}