#include "actor.h"
#include "renderables.h"

#include <math.h>



actor actor_pool[MAX_ACTOR];


void default_render(const actor* a)
{
	glPushMatrix();
	glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
	glRotatef(atan2f(a->vel[0], a->vel[1]), 0.0f, 0.0f, 1.0f);
	glRotatef(a->ang, 0.0f, 1.0f, 0.0f);
	glScalef(0.2f,1.0f,0.2f);
	//glScalef(0.05f,0.05f,0.05f);
	cube_w();
	glPopMatrix();
}


void default_update(actor* a, float dt)
{
	MADD(a->pos, a->pos, dt, a->vel);
	a->ang += 4000.0f*dt;
}

/// reset de actor
void ACTOR_reset(actor* a)
{
	a->flags = 0;
	a->life = 1.0f;
	a->update = default_update;
	a->render = default_render;
	a->type = 0;
}

void ACTOR_init(actor* pool)
{
	int i;
	for(i = 0; i < MAX_ACTOR; ++i)
	{
		ACTOR_reset(&pool[i]);
	}
}

void ACTOR_update(actor* pool, float dt)
{
	int i;
	for(i = 0; i < MAX_ACTOR; ++i)
	{
		actor* a = &pool[i];
		if(a->flags & F_ACTIVE)
		{
			a->time += dt;
			if (a->update)
				a->update(a, dt);
			else
				default_update(a, dt);
		}
	}
}

actor* ACTOR_get(actor* pool)
{
	int i;
	for(i = 0; i < MAX_ACTOR; ++i)
	{
		actor* a = &pool[i];
		if(IS_ACTIVE(a) == 0)
		{
			a->flags = F_ACTIVE;
			a->time = 0.0f;			
			return a;
		}
	}
	return 0;
}



void ACTOR_render(actor* pool)
{	
	int i;
	for(i = 0; i < MAX_ACTOR; ++i)
	{
		actor* a = &pool[i];
		if(IS_ACTIVE(a))
		{
			if(a->render)
			{
				a->render(a);
			}
			else
			{
				default_render(a);
			}
		}
	}
}

void ACTOR_kill(actor* a) 
{
	ACTOR_reset(a);
}



int ACTOR_collide(const actor* a,const float* pos)
{
	return in_range(a->pos[0], a->collide_size[0], pos[0]) &&
			in_range(a->pos[1], a->collide_size[1], pos[1]);
}