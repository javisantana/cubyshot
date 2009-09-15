#include "renderables.h"
#include "actor.h"
#include "random_ship.h"

void final_boss_init(struct actor_t* a)
{
	a->pos[2] = -100.0f;
	SHIP_generate(a, randi(12312312), 13, 6);
}
void final_boss_update(struct actor_t* a, float)
{
	
	a->ang += 40.0f*dt;
	a->pos[2] -= a->pos[2]*0.07f;
	a->pos[0] = 10.0f*sinf(a->time*0.4f);
	a->pos[1] =  14.0f + 3.0f*sinf(a->time*0.1f);
	MADD(a->pos, a->pos, dt, a->vel);
}
void final_boss_render(const struct actor_t* a)
{
	SHIP_render(a);
	return;
	glPushMatrix();
	glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
	glRotatef(atan2f(a->vel[0], a->vel[1]), 0.0f, 0.0f, 1.0f);
	glRotatef(a->ang, 0.0f, 1.0f, 0.0f);
	//glScalef(0.2f,1.0f,0.2f);
	//glScalef(0.05f,0.05f,0.05f);
	cube_w();
	glPopMatrix();
}


