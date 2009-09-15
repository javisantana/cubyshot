#include <windows.h>
#include <gl/gl.h>

#include "math.h"
#include "random_ship.h"
#include "renderables.h"


void ship_render(const struct random_ship* s); 

void SHIP_render(const struct actor_t* e)
{
	random_ship_t* rs = (random_ship_t*)e->child;
	glPushMatrix();
	glTranslatef(e->pos[0], e->pos[1], e->pos[2]);
	glRotatef(atan2f(e->vel[0], e->vel[1]), 0.0f, 0.0f, 1.0f);
	//glRotatef(a->ang, 0.0f, 1.0f, 0.0f);
	glScalef(0.5f, 0.5f, 0.5f);
	ship_render(rs);
	glPopMatrix();
	
}


void SHIP_generate(struct actor_t* a, int _seed, int x, int y)
{
		struct random_ship* s = (random_ship_t*)malloc(sizeof(random_ship_t));
		int i,j;
		int old_seed = seed;
		seed = _seed;
		s->w = x;
		s->h = y;
		s->rot = 0.0f;
		for( i = 0; i < 1 + (s->w >> 1); ++i)
		{
			for( j = 0; j < s->h; ++j)
			{
				float jf = ((float)j)/s->h;
				s->cells[i][j] = rand01() < 0.5f ? 1:0;
			}
		}
		//mirror
		for(i = 0; i < (s->w >> 1); ++i)
		{
			for( j = 0; j < s->h; ++j)
			{
				s->cells[s->w - i - 1][j] = s->cells[i][j] ;
			}
		}

		//rotation
		for( j = 0; j < s->h; ++j)
		{
			//if(!s->cells[s->w >> 1][j] && rand01() < 0.6f)
			//s->cells[s->w >> 1][j] = 2;
		}
		a->child = s;
		seed = old_seed ;
}

void ship_render(const struct random_ship* s)
{
	int i,j;
	//glEnable(GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	//glDisable(GL_CULL_FACE);
	
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glColor4f(0.0f,0.0f,0.0f, 0.1f);

	glPushMatrix();

	glRotatef(s->rot, 0.0f, 0.0f, 1.0f);
	//glScalef(0.5f, .5f, .5f);

	for(i = 0; i < s->w; ++i)
	{
			for(j = 0; j < s->h; ++j)
			{
					glPushMatrix();
					if(s->cells[s->w >> 1][j] == 2)
					{
						glRotatef(s->rot*20.0f, 0.0f, 1.0f, 0.0f);
					}
					if(s->cells[i][j] == 1)
					{
						glPushMatrix();
						
						glTranslatef(2.0f*(i - (s->w >> 1)) , 2.0f*j, 0.0f);
						glScalef(0.9f, 0.9f, 0.9f);
						cube_w();
						glPopMatrix();
					}				
					glPopMatrix();
			}
	}
	//return;
	
	for(i = s->w >> 1; i < s->w ; ++i)
	{
			for(j = 0; j < s->h>>2; ++j)
			{
					glPushMatrix();
					if(s->cells[s->w >> 1][j] == 2)
					{
						glRotatef(s->rot*20.0f, 0.0f, 1.0f, 0.0f);
					}

					if(s->cells[i][j])
					{
						if( i != s->w >> 1)
						{
							glTranslatef( 0.0f, 2.0f*j , 2.0f*i - (s->h>>1) - 1 );
							glScalef(0.9f, 0.9f, 0.9f);
							cube_w();
							
						}
					}				
					glPopMatrix();
			}
	}
	
	glPopMatrix();
	//render_doids(player_seed);
}


