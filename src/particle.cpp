#include "renderables.h"
#include "math.h"
#include "actor.h"



actor particle_pool[MAX_ACTOR];
void PART_update(actor* a, float dt)
{
	MADD(a->pos, a->pos, dt, a->vel);
	a->ang += 4000.0f*dt;
	a->aux[0] -= 1.2f*dt;
	if(a->aux[0] < 0.0f)
	{
		ACTOR_kill(a);
	}
}

void PART_render(const actor* a)
{
	int p =0;


		if(a->aux[0] > 0)
		{
			//TODO: i expect that compiled optimize this accesses
			//TODO: learn english to write todos
			//girar cara a la cámara
			//glEnable(GL_BLEND);
			float life = a->aux[0];
			glPushMatrix();

			glTranslatef(a->pos[0],a->pos[1],a->pos[2]);
			glRotatef(a->aux[0]*1000.0f,0,1,0);
			glRotatef(a->aux[0]*1600.0f,0,0,1);
			glScalef(life, life, life);			
			cube_w();
			glPopMatrix();			

		}

	
	//glDisable(GL_BLEND);
}
void PART_damage(float* pos, float velmul)
{
	int i;
	for( i = 0;i < 5;i++)
	{
		actor* p = ACTOR_get(particle_pool);
		p->update = PART_update;
		p->render = PART_render;
	
		VMOV(p->pos, pos);
		VMOV3(p->vel, randf()*5.0f, velmul*rand01()*55.0f, randf()*2.0f);

		p->aux[0] = 0.1f+ 0.2f*rand01(); //aux[0] life

	}
}
void PART_explosion(float* pos, int n)
{

	int i;
	for( i = 0;i < n;i++)
	{
		actor* p = ACTOR_get(particle_pool);
		p->update = PART_update;
		p->render = PART_render;
	/*	p->color[0] = 0;
		p->color[1] = 0;
		p->color[2] = 0.8f;
		p->color[3] = 0.5f;
*/
		p->pos[0] = pos[0];
		p->pos[1] = pos[1];
		p->pos[2] = pos[2];

		p->vel[0] = randf()*15.0f;
		p->vel[1] = randf()*15.0f;
		p->vel[2] = randf();

		p->aux[0] = 0.2f+ 0.6f*rand01(); //aux[0] life

	}

}

actor* PART_get()
{
	actor* p = ACTOR_get(particle_pool);
		p->update = PART_update;
		p->render = PART_render;
		return p;
}


void PART_cube_destroy(float* pos, float s)
{
	int i,j,k;
	int n = int(s*2.0f);
	float s2 = s*0.5f;
	for( i = 0;i < n;i++)
	{
		for( j = 0;j < n;j++)
		{
			for( k = 0;k < n;k++)
			{	
				actor* p = ACTOR_get(particle_pool);
				p->update = PART_update;
				p->render = PART_render;
		
				p->pos[0] = pos[0] + 0.1f*i - s2;
				p->pos[1] = pos[1] + 0.1f*j - s2;
				p->pos[2] = pos[2] + 0.1f*k - s2;

				p->vel[0] = randf()*0.1f;
				p->vel[1] = 0.f*rand01();
				p->vel[2] = -0.1f*rand01();

				p->aux[0] = 0.2f+ 0.6f*rand01();
			}
		}
	

	}
}
