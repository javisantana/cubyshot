#include "bullet.h"
#include "actor.h"
#include "renderables.h"

float bullet_colors[2][4] = 
{ 
			{ 0.0f, 0.3f, 0.0f, 0.4f} , //player
			{ 0.8f, 0.0f, 0.0f, 0.3f}   // enemy
};
void BULLET_render(const actor* a)
{
	glPushMatrix();
	glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
	//glRotatef(atan2f(a->vel[0], a->vel[1]), 0.0f, 0.0f, 1.0f);
	glRotatef(a->ang, 1.0f, 1.0f, 0.0f);
	glScalef(0.3f,0.3f,0.3f);
	//glScalef(0.05f,0.05f,0.05f);
	glColor4fv(bullet_colors[a->type - BULLET]);
	cube_w();
	glPopMatrix();
	glColor4f(0.3f, 0.3f, 0.3f, 0.4f);
}


void BULLET_shot(vec3f pos, vec3f vel, int type)
{
	actor* a = ACTOR_get(actor_pool);
	VMOV(a->pos, pos);
	VMOV(a->vel, vel);
	
	a->collide_size[0] = 1.0f;
	a->collide_size[1] = 1.0f;
	a->type = type;
	
	if(type == BULLET)
	{
		MAKE_ENEMY_HURT(a);
	}
	else
	{
		MAKE_PLAYER_HURT(a);
	}
	a->render = BULLET_render;
	a->flags |= F_BULLET;

}

void BULLET_random_bullet(vec3f pos, vec3f vel)
{
		vec3f v;
		VMOV3(v, vel[0]*rand01(), -20.0f, 0.0f);
		BULLET_shot(pos,v,  BULLET_ENEMY);
}