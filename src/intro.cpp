//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "intro.h"
#include "mzk.h"

#include "actor.h"
#include "renderables.h"

const float dt = 0.02f;

actor actor_pool[MAX_ACTOR];

//---------------------------------------------------------------------

float pj_pos[2];
float pj_vel[2];
int pj_input[5];
int pj_fire_cnt = 0;
float pj_angle = 0.0f;
int pj_engine_smoke = 0;
int pj_powering = 0;
int pj_old_powering = 0;
float pj_ang = 0.0f;

const int PJ_FIRE_RATE = 4;
const int PJ_ENGINE_SMOKE_RATE = 6;


void render_pj()
{
	int i;
	glPushMatrix();
	glTranslatef(pj_pos[0], pj_pos[1], 0.0f);
	glRotatef(pj_angle, 0.0f, 1.0f, 0.0f);
	glColor3f(0.0f,0.0f,0.0f);
	//center
	cube_w();
	for(i = 1; i < 3; ++i)
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
		
	glPopMatrix();
}

void update_pj()
{
	//pj_vel[0] += ((pj_input[2] - pj_input[3])*20.0f - pj_vel[0])*40.5f*dt;
	pj_vel[0] = (pj_input[2] - pj_input[3])*20.0f;// - pj_vel[0])*40.5f*dt;
	pj_vel[1] = (pj_input[0] - pj_input[1])*20.0f;// - pj_vel[0])*40.5f*dt;
	pj_pos[0] -= pj_vel[0]*dt;
	pj_pos[1] += pj_vel[1]*dt;

	pj_angle = -pj_vel[0];

	pj_ang += 6.0f*dt;

	

	//shots
	if(pj_input[4] && pj_fire_cnt-- == 0)
	{
		int i;
		for( i = 0; i < 3; ++i)
		{
			actor* a = ACTOR_get(actor_pool);
			
			a->pos[0] = pj_pos[0];
			a->pos[1] = pj_pos[1];
			a->pos[2] = 0.0f;

			a->vel[0] = i - 1.5f;
			a->vel[0]*=2.0f;

			a->vel[1] = 35.0f;
			a->vel[2] = 0.0f;
			
			a->type = BULLET;
			//a->flags |= F_ACTIVE; 
			pj_fire_cnt = PJ_FIRE_RATE;
		}
	}

	//engines
	/*
	if(pj_vel[1] > 0.0f || pj_engine_smoke-- == 0)
	{
		const float pp[] = { -2.0f, 2.0f }; 
		int i = 0;
		for(i = 0; i < 2; ++i)
		{
			struct particle* p = get_particle();
			p->color[0] = 0;
			p->color[1] = 0;
			p->color[2] = 0.8f;
			p->color[3] = 0.5f;

			p->pos[0] = pj_pos[0] + pp[i];
			p->pos[1] = pj_pos[1] - 1.0f;
			p->pos[2] = pj_pos[2];

			p->dir[0] = randf()*0.1f;
			p->dir[1] = -2.0f -rand01()*5.0f;// - pj_vel[1];
			p->dir[2] = 0.0f;
		

			p->life = 0.2f+ 0.3f*rand01();
		}
		pj_engine_smoke = PJ_ENGINE_SMOKE_RATE;
	}
	*/

	
}

// --- input 
void input()
{

	pj_input[0] = GetAsyncKeyState(VK_UP) ? 1:0;
	pj_input[1] = GetAsyncKeyState(VK_DOWN) ? 1:0;
	pj_input[2] = GetAsyncKeyState(VK_LEFT) ? 1:0;
	pj_input[3] = GetAsyncKeyState(VK_RIGHT) ? 1:0;
	pj_input[4] = GetAsyncKeyState('A') ? 1:0;
	pj_old_powering = pj_powering;
	pj_powering = GetAsyncKeyState('S') ? 1:0;
	
  	
}

void update()
{

	update_pj();
	ACTOR_update(actor_pool, dt);	
	update_pj();

}

void render()
{
	glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glTranslatef(0.0f,0.0f, -40.0f);
	glColor3f(1.0f,1.0f,1.0f);

	render_pj();

	ACTOR_render(actor_pool);
}
//----------------------------------------

#define fzn  0.005f
#define fzf  1000.0f

void intro_init( void )
{
	static const float projectionmatrix[16] = {
    1.0f, 0.00f,  0.0f,                    0.0f,
    0.0f, 1.25f,  0.0f,                    0.0f,
    0.0f, 0.00f, -(fzf+fzn)/(fzf-fzn),    -1.0f,
    0.0f, 0.00f, -2.0f*fzf*fzn/(fzf-fzn),  0.0f };

    glDisable(GL_DEPTH_TEST);
	glDisable( GL_BLEND );
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( projectionmatrix );
    glMatrixMode( GL_MODELVIEW );
//	back_init();
	ACTOR_init(actor_pool);
	cube_init();
	
	
}



void intro_do( long itime )
{
   	input();
	update();

	
	render();
    
}

