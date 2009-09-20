#include "actor.h"
#include "renderables.h"
#include "bullet.h"
#include "board.h"
#include "snd/effects.h" 
#include "particle.h" 
int pj_input[5];
int pj_powering = 0;
int pj_old_powering = 0;
float pj_pos[2];
float pj_vel[2];
float pj_life ;


int pj_fire_cnt = 0;
int pj_engine_smoke = 0;
float pj_angle = 0.0f;
float pj_ang = 0.0f;
int pj_score = 0;

int pj_fire_count = 2;
const int PJ_FIRE_RATE = 4;
const int PJ_ENGINE_SMOKE_RATE = 6;

void ship_render()
{
	cube_w();
	for(int i = 1; i < 3; ++i)
	{
		float s = 1.0f - ((float)i)/3.0f;
		
		float d = s + 3.5f*((float)i)/3.0f;
		
		glPushMatrix();
		glTranslatef(d, - 1.0f +  s, 0.0f);
		glScalef(s, s, s);
		cube_w();
		glPopMatrix();

		glPushMatrix();
		glTranslatef(-d, - 1.0f +  s, 0.0f);
		glScalef(s, s, s);
		cube_w();
		glPopMatrix();

	}
}
void PLAYER_render()
{
	
	glPushMatrix();
	glTranslatef(pj_pos[0], pj_pos[1], 0.0f);
	glRotatef(pj_angle, 0.0f, 1.0f, 0.0f);
	glColor4f(0.0f,0.0f,0.0f, 0.2f);
	//center
	ship_render();
		
	glPopMatrix();
}

void PLAYER_init()
{
	pj_life = 1.0f;
	
}

void PLAYER_kill()
{

	const float pp[] = { 0.0f, -2.0f, 2.0f }; 
	int i = 0;
	for(i = 0; i < 4; ++i)
	{
		actor* p = PART_get();

	
		p->pos[0] = pj_pos[0] + pp[i];
		p->pos[1] = pj_pos[1] - 1.0f;
		p->pos[2] = pj_pos[2];

		p->vel[0] = randf()*10.0f;
		p->vel[1] = randf()*10.0f;
		p->vel[2] = 0.0f;

		p->aux[0] = 1.0f;

	}
	PART_explosion(pj_pos, 10);
	
}

void PLAYER_update()
{
	//pj_vel[0] += ((pj_input[2] - pj_input[3])*20.0f - pj_vel[0])*40.5f*dt;
	pj_vel[0] = (pj_input[2] - pj_input[3])*30.0f;// - pj_vel[0])*40.5f*dt;
	pj_vel[1] = (pj_input[0] - pj_input[1])*30.0f;// - pj_vel[0])*40.5f*dt;

	
	pj_pos[0] -= pj_vel[0]*dt;
	pj_pos[1] += pj_vel[1]*dt;
	
	if(!BOARD_inside(pj_pos)) 
	{
		pj_pos[0] += pj_vel[0]*dt;
		pj_pos[1] -= pj_vel[1]*dt;
	}

	
	pj_angle = -pj_vel[0];

	pj_ang += 6.0f*dt;

	

	//shots
	if(pj_input[4] && pj_fire_cnt-- == 0)
	{
		
		int i;
		vec3f v;
		 
		
		{
			for( i = 0; i < 3; ++i)
			{
				//actor* a = ACTOR_get(actor_pool);
				
				VMOV3(v,2.0f*i - 3.0f, 55.0f, 0.0f);		

				BULLET_shot(pj_pos, v, BULLET);

				
				/*a->pos[0] = pj_pos[0];
				a->pos[1] = pj_pos[1];
				a->pos[2] = 0.0f;

				a->vel[0] = i - 1.5f;
				a->vel[0]*=2.0f;

				a->vel[1] = 55.0f;
				a->vel[2] = 0.0f;
				
				a->collide_size[0] = 1.0f;
				a->collide_size[1] = 1.0f;
				a->type = BULLET;
				*/
				
				
			}
		}
		if(pj_fire_count == 3)
		{
			vec3f vel;
			VMOV3(vel, -9.0f, 55.0f, 0.0f);
			VMOV(v, pj_pos);
			v[0] -= 2.0f;
			BULLET_shot(v, vel, BULLET);

			VMOV3(vel, 9.0f, 55.0f, 0.0f);			
			v[0] += 4.0f;
			BULLET_shot(v, vel, BULLET);
		}
		else if(pj_fire_count >= 4)
		{
			vec3f p;
			VMOV(p, pj_pos);
			p[0] -= 3.0f;
			for( i = 0; i < 3; ++i)
			{	
				VMOV3(v,2.0f*i - 3.0f, 55.0f, 0.0f);				
				VMOV3(v,-4.0f*(8.0f*i - 12.0f), 55.0f, 0.0f);	
				BULLET_shot(p, v, BULLET);
			}
			p[0] += 6.0f;
			for( i = 0; i < 3; ++i)
			{	
				VMOV3(v,4.0f*(8.0f*i - 12.0f), 55.0f, 0.0f);						
				BULLET_shot(p, v, BULLET);
			}
		}
		pj_fire_cnt = PJ_FIRE_RATE;
		EFFECTS_player_shoot();
	}

	//engines
	
	if(pj_vel[1] > 0.0f || pj_engine_smoke-- == 0)
	{
		float damage = 1.0f - pj_life; // 0 -> 1
		const float pp[] = { -2.0f, 2.0f }; 
		int i = 0;
		for(i = 0; i < 2; ++i)
		{
			
			actor* p = PART_get();

			p->pos[0] = pj_pos[0] + pp[i];
			p->pos[1] = pj_pos[1] - 1.0f;
			p->pos[2] = pj_pos[2];

			p->vel[0] = randf()*0.1f;
			p->vel[1] = -2.0f -rand01()*5.0f;// - pj_vel[1];
			p->vel[2] = 0.0f;
		

			p->aux[0] = 0.2f+ 0.3f*rand01();
		}
		if(rand01() < damage*0.2f)
		{
			for(i = 0; i < 5; ++i)
			{
				actor* p = PART_get();

				p->pos[0] = pj_pos[0] + 2.0f*randf();
				p->pos[1] = pj_pos[1] +  randf();
				p->pos[2] = pj_pos[2];

				p->vel[0] = randf();
				p->vel[1] = -2.0f - rand01()*5.0f;// - pj_vel[1];
				p->vel[2] = 0.0f;
			

				p->aux[0] = 0.4f+ 0.3f*rand01();
			}
		}
		pj_engine_smoke = PJ_ENGINE_SMOKE_RATE;
	}

	if(pj_life < 0.0f)
	{
		PLAYER_kill();
	}
	
	

	
}
