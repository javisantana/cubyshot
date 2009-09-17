#include "renderables.h"
#include "actor.h"
#include "random_ship.h"

/// final boss collision are managed in diferent way
bool final_boss_collide(struct actor_t* a, struct actor_t* bullet)
{
	if(SHIP_collide(a, bullet->pos))
	{
		return true;
	}
	return false;


}
void final_boss_init(struct actor_t* a)
{
	a->pos[2] = -100.0f;
	SHIP_generate(a, randi(12312312), 13, 6);
	a->collide_size[0] = 13.0f;
	a->collide_size[1] = 4.0f;
}

void final_boss_update(struct actor_t* a, float)
{
	a->ang += 40.0f*dt;
	a->pos[2] -= a->pos[2]*0.07f;
	a->pos[0] = 10.0f*perlin2d(a->time*0.001f, a->time*0.001f)*sinf(a->time*0.4f);
	a->pos[1] =  14.0f + 3.0f*sinf(a->time*0.4f);
	a->ang = 5.0f*sinf(a->time);
	MADD(a->pos, a->pos, dt, a->vel);
	SHIP_update(a);
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


