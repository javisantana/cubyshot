#pragma once

#include "math.h"

// actor - bullets, ships...
enum { BULLET, BULLET_ENEMY, //bullets

       ENEMY_1 };

#define F_ACTIVE 0x01
#define F_COLLIDE_PLAYER 0x02 // collide with player
#define F_COLLIDE_ENEMY 0x04 // collide with enemy
#define F_BULLET 0x08

#define IS_ACTIVE(a) ((a)->flags & F_ACTIVE)
#define MAKE_PLAYER_HURT(a) ((a)->flags |= F_COLLIDE_PLAYER)
#define MAKE_ENEMY_HURT(a) ((a)->flags |= F_COLLIDE_ENEMY)

typedef struct actor_t
{
	float pos[3];
	float vel[3];
	float ang;
	int flags;
	int type;
	float collide_size[2];
	void (*update)(struct actor_t*, float);
	void (*render)(const struct actor_t*);	
	float life;
	float time;
	float aux[3];
	void* child;
	//float ang;

	/*
	
	float acel[3];
	
	int seed;
	*/


} actor;


#define MAX_ACTOR 512

extern actor actor_pool[MAX_ACTOR];
const float dt = 0.02f;

void ACTOR_init(actor*);
void ACTOR_update(actor*, float dt);
void ACTOR_render(actor*);
void ACTOR_kill(actor* a);
int ACTOR_collide(const actor* a,const float* pos);
actor* ACTOR_get(actor* pool);



