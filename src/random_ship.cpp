#include <windows.h>
#include <gl/gl.h>

#include "math.h"
#include "random_ship.h"
#include "renderables.h"
#include "particle.h"

void ship_render(const struct random_ship* s); 

void kamikaze_update(struct actor_t* a, float dt)
{
	MADD(a->pos, a->pos, dt, a->vel);
	a->ang += a->aux[0] *dt;
	a->vel[2]*=0.999f;
}
void kamikaze_render(const struct actor_t* a)
{
	glPushMatrix();
	glTranslatef(a->pos[0], a->pos[1], a->pos[2]);	
	glRotatef(a->ang, 0.0f, 1.0f, 1.0f);
	glScalef(a->collide_size[0],a->collide_size[0],a->collide_size[0]);
	//glScalef(0.05f,0.05f,0.05f);
	cube_w();
	glPopMatrix();
}
void kamikaze_add(vec3f pos, float s)
{
	actor* a = ACTOR_get(actor_pool);
			
	VMOV(a->pos, pos);
	

	a->vel[0] = 0.0f;
	a->vel[1] = -rand01();
	a->vel[2] = -5.0f  -10.0f*rand01();

	a->ang = 0.0;	
	a->collide_size[0] = s;
	a->collide_size[1] = s;
	a->aux[0] = randf()*30.0f;
	

	
	a->update = kamikaze_update;
	a->render = kamikaze_render;
	
}


void SHIP_update(struct actor_t* a)
{
	int i,j;
	random_ship_t* s = (random_ship_t*)a->child;
	for( i = 0; i < s->w; ++i)
	{
		for( j = 0; j < s->h; ++j)
		{
			if(s->cells[i][j] > 0.0f)
			{
				if(s->celllife[i][j] < 0.0f)
				{
					vec3f cellpos;
					VMOV3(cellpos, 1.0f*(i - (s->w >> 1)) , 1.0f*j, 0.0f);
					VADD(cellpos, a->pos, cellpos);
					kamikaze_add(cellpos, s->cells[i][j]*0.5f);
					s->cells[i][j] = 0.0f; //kill cell
				} 
				else if(s->cellsize[i][j] > s->cells[i][j])
				{
					s->cellsize[i][j] -= (s->cellsize[i][j] - s->cells[i][j])*0.1f;					
				}
			}
		}
	}
}
bool SHIP_collide(struct actor_t* a, vec3f pos)
{
		int i,j;
		random_ship_t* s = (random_ship_t*)a->child;
		for( i = 0; i < s->w; ++i)
		{
			for( j = 0; j < s->h; ++j)
			{
				vec3f cellpos;
				VMOV3(cellpos, 1.0f*(i - (s->w >> 1)) , 1.0f*j, 0.0f);
				VADD(cellpos, a->pos, cellpos);
				float cellsize = s->cells[i][j];
				if(in_range(cellpos[0], cellsize, pos[0]) &&
					in_range(cellpos[1], cellsize, pos[1]))
				{
					if(s->celllife[i][j] > 0.0f)
					{
						s->cellsize[i][j] = s->cells[i][j]*1.3f;
					}
					//TODO: hard
					s->celllife[i][j]-= 0.1f;
					return true;
				}
			}
		}
		return false;
}


void SHIP_render(const struct actor_t* e)
{
	random_ship_t* rs = (random_ship_t*)e->child;
	glPushMatrix();
	glTranslatef(e->pos[0], e->pos[1], e->pos[2]);
	glRotatef(atan2f(e->vel[0], e->vel[1]), 0.0f, 0.0f, 1.0f);
	glRotatef(e->ang, 0.0f, 0.0f, 1.0f);
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
				s->cellsize[i][j] = s->cells[i][j] = rand01() < 0.5f ? 0.2f + 2.7f*rand01():0.0f;
			}
		}
		//mirror
		for(i = 0; i < (s->w >> 1); ++i)
		{
			for( j = 0; j < s->h; ++j)
			{
				
				s->cellsize[s->w - i - 1][j] = s->cells[s->w - i - 1][j] = s->cells[i][j] ;
			}
		}


		for( i = 0; i < s->w; ++i)
		{
			for( j = 0; j < s->h; ++j)
			{

				if(s->cells[i][j] > 0.0f)
				{
					s->celllife[s->w - i - 1][j] = s->celllife[i][j] = s->cells[i][j];
				}
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
				/*	if(s->cells[s->w >> 1][j] == 2)
					{
						glRotatef(s->rot*20.0f, 0.0f, 1.0f, 0.0f);
					}
					*/
					if(s->cells[i][j] > 0.0f)
					{
						float d = s->cellsize[i][j] - s->cells[i][j];
						if(d > 0.01f)
							glColor4f(0.1f+d*1.5f,0.0f,0.0f, 0.4f);
						else
							glColor4f(0.0f,0.0f,0.0f, 0.1f);
						glPushMatrix();
						float ss = s->cellsize[i][j];
						glTranslatef(2.0f*(i - (s->w >> 1)) , 2.0f*j, 0.0f);
						glScalef(ss, ss, ss);
						cube_w();
						glPopMatrix();
					}				
					glPopMatrix();
			}
	}
	return;
	
	for(i = s->w >> 1; i < s->w ; ++i)
	{
			for(j = 0; j < s->h>>2; ++j)
			{
					glPushMatrix();
				/*	if(s->cells[s->w >> 1][j] == 2)
					{
						glRotatef(s->rot*20.0f, 0.0f, 1.0f, 0.0f);
					}
*/
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


