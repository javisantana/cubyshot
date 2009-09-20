#include "renderables.h"
#include "math.h"
#define _RGB(r,g,b) r.0f/255.0f, g.0f/255.0f, b.0f/255.0f

float palete[3][5][3] = 
{
	{
			{_RGB(11, 0, 0)},
			{_RGB(33, 0, 0)},
			{_RGB(90, 0, 0)},
			{_RGB(136, 0, 0)},
			{_RGB(180, 0, 0)}
	},
	{
			{_RGB(11, 44, 51)},
			{_RGB(33, 83, 84)},
			{_RGB(90, 156, 154)},
			{_RGB(136, 224, 225)},
			{_RGB(180, 237, 230)}
	},

	{
			{_RGB(190, 150, 29)},
			{_RGB(201, 167, 83)},
			{_RGB(203, 190, 137)},
			{_RGB(203, 200, 193)},
			{_RGB(227, 229, 210)}
	},
	
};

vec3f color[5];
int curr = 0;



float* pal_color(int i)
{
	return color[i];
}

void set_pal_color(int i, float alpha)
{
	
	float* c = pal_color(i);
	glColor4f(c[0], c[1], c[2], alpha);
}

void pal_update()
{
	for(int i = 0; i < 5; ++i)
	{	
		float* to = palete[curr][i];
		color[i][0] += (to[0] - color[i][0])*0.05f;
		color[i][1] += (to[1] - color[i][1])*0.05f;
		color[i][2] += (to[2] - color[i][2])*0.05f;

	}
}

void pal_change_to(int i)
{
	curr = i;

}