#include "renderables.h"
#include "actor.h"
#include "random_ship.h"
#include "bullet.h"
#include "particle.h"

struct boss_timers_t
{
	int global;
	int burst;
	int inter_burst;

	int cburst;
	int cinter_burst;

	int type;
};

enum { FIRE_RANDOM, FIRE_STAR };

struct boss_timers_t boss_timers;
/// final boss collision are managed in diferent way
bool final_boss_collide(struct actor_t* a, struct actor_t* bullet)
{
	if(SHIP_collide(a, bullet->pos))
	{
		a->life -= 0.001f;
		return true;
	}
	return false;
}

void boss_random_fire(struct actor_t* a, int n)
{
		random_ship_t* s = (random_ship_t*)a->child;
		int last = 0;
		for(int i = 0; i < s->w; ++i)
		{
			if(s->cells[i][last] > 0.0f)
			{
				vec3f p;
				SHIP_cell_pos(a, i, last, p);
				//BULLET_random_bullet(p,a->vel);
				for(int j = 0; j < n; ++j)
				{
					vec3f vel;
					float ang = -0.5f*PI - linear(-PI/2.0f, PI/2.0f, float(j)/float(n));
					vel[0] = 20.0f*cosf(ang);
					vel[1] = 20.0f*sinf(ang);
					vel[2] = 0.0f;
					BULLET_random_bullet(p,vel);
				}
				PART_damage(p, -0.2f);

			}
		}
}

void bullet_star(vec3f p, float time_factor)
{
	for(int i = 0; i < 10; ++i)
	{
		vec3f vel;
		float ang = time_factor + linear(-PI, PI, float(i)/10.0f);
		vel[0] = 20.0f*cosf(ang);
		vel[1] = 20.0f*sinf(ang);
		vel[2] = 0.0f;
		BULLET_shot(p, vel, BULLET_ENEMY);
	}
	
}


void boss_star_fire(struct actor_t* a, float time_factor)
{
		random_ship_t* s = (random_ship_t*)a->child;		
		
		vec3f p;
		if(s->cells[s->gun_first][0] > 0.0f)
		{
			SHIP_cell_pos(a, s->gun_first, 0, p);		
			bullet_star(p, a->time*time_factor);		
			PART_damage(p, -0.2f);
		}
		if(s->cells[s->gun_last][0] > 0.0f)
		{
			SHIP_cell_pos(a, s->gun_last, 0, p);		
			bullet_star(p, a->time*time_factor);		
			PART_damage(p, -0.2f);
		}
}
	
//manage all bullet control
void final_boss_fire(struct actor_t* a)
{
	random_ship_t* s = (random_ship_t*)a->child;

	if(boss_timers.global-- == 0)
	{
		boss_timers.burst = 2 + randi(12);
		boss_timers.cburst = 0;
		boss_timers.inter_burst = 2 + randi(30);
		boss_timers.cinter_burst = boss_timers.inter_burst;
		boss_timers.type = randi(2) == 0?FIRE_RANDOM: FIRE_STAR;

		boss_timers.global = 90 + randi(200);
    }

	boss_timers.cinter_burst--;
	if(boss_timers.cinter_burst == 0)
	{
		boss_timers.burst--;
		if(boss_timers.burst >= 0)
		{
			switch(boss_timers.type)
			{
				case FIRE_RANDOM:
					boss_random_fire(a,3 );
					break;
				case FIRE_STAR:
					boss_star_fire(a, 1.0f);
					break;

			}

			boss_timers.cinter_burst = boss_timers.inter_burst;
		}
	}




}


void final_boss_init(struct actor_t* a)
{
	a->pos[2] = -100.0f;
	SHIP_generate(a, randi(12312312), 13, 6);
	a->collide_size[0] = 13.0f;
	a->collide_size[1] = 4.0f;

	boss_timers.global = 0;
	boss_timers.burst = 0;
	boss_timers.inter_burst = 0;

	
	
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
	a->count++;
	final_boss_fire(a);


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


