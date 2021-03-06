#include "math.h"
#include "actor.h"
//#include <math.h>
#include "enemy.h"
#include "board.h"
#include "bullet.h"
#include "renderables.h"
#include "player.h"

#include "boss.h"
#include "palete.h"
#include "snd/sound.h"

enum { FIXED_POS, RANDOM_POS};
enum { TOP, SIDE_L, SIDE_R };


typedef struct 
{
	//data
	int group_interval;
	int interval;
	int group_num;

	int sequence; // the ships appears in same point? (FIXED, RANDOM)
	int side;	  // place where ship appears, TOP, SIDE 

	float fixed_pos;
	float fixed_ang;
	//counters
	int next_cnt;
	int num_cnt;

	void (*update_fn)(struct actor_t*, float);
	void (*render_fn)(const struct actor_t*);

	float aux[3];
	int ship_type;
	int count;


} enemy_pattern;

#define MAX_PATTERNS 12
enemy_pattern patterns[MAX_PATTERNS];
int enemy_patterns = 0;
/*= 
{ 
	{ 300, 10, 6, 6, 1},
    { 200, 80, 2, 200, 2}
};
*/


extern void ship_render();

void ship_medium_render_mobile(const actor* a)
{
	glPushMatrix();
	
	glColor4f(0.0f,0.0f,0.0f, 0.2f);	
	glTranslatef(a->pos[0], a->pos[1], a->pos[2]);	

	glPushMatrix();
		glScalef(0.4f,0.4f,0.4f);
		cube_w();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(1.5f, 0.9f, -0.5f);	
		glScalef(1.4f,1.4f,0.2f);
		cube_w();

		

	glPopMatrix();
	//ca�on
	glPushMatrix();
		glScalef(0.4f,0.4f,0.4f);	
		glTranslatef(3.5f, 0.9f, 0.0f);
		glRotatef(a->ang, 0.0f, 0.0f, 1.0f);	
		glScalef(1.0f,1.0f,1.0f);	
		cube_w();
		
	glPopMatrix();		
		

	glPushMatrix();
		glTranslatef(-1.5f, 0.9f, -0.5f);	
		glScalef(1.4f,1.4f,0.2f);
		cube_w();
	glPopMatrix();

	glPushMatrix();
		glScalef(0.4f,0.4f,0.4f);	
		glTranslatef(-3.5f, 0.9f, 0.0f);
		glRotatef(a->ang, 0.0f, 0.0f, 1.0f);	
		glScalef(1.0f,1.0f,1.0f);	
		cube_w();		
	glPopMatrix();



	glPopMatrix();



}
void ship_medium_render(const actor* a)
{

	GLfloat density = 0.02f;
	GLfloat fogColor[4] = {1.0f, 1.0f, 1.0f, 1.0f}; 

	glFogi (GL_FOG_MODE, GL_EXP2);	
	glFogfv (GL_FOG_COLOR, fogColor);
	glFogf (GL_FOG_DENSITY, density);
	glHint (GL_FOG_HINT, GL_NICEST);

	glEnable(GL_FOG);
	glPushMatrix();
	
	glColor4f(0.0f,0.0f,0.0f, 0.2f);	
	glTranslatef(a->pos[0], a->pos[1], a->pos[2]);	
		glPushMatrix();
		glTranslatef(0.0f, 0.0f, -50.0f);	
		glScalef(2.0f,2.0f,50.0f);
		cube_w();
		glPopMatrix();
	glDisable(GL_FOG);
	
	glRotatef(a->ang, 0.0f, 0.0f, 1.0f);	
	glScalef(1.0f,1.0f,1.0f);	
	cube_w();
	glTranslatef(1.5f, 0.0f, 0.0f);
	glScalef(1.0f, 0.2f, 0.2f);
	cube_w();
	glPopMatrix();

	

}

void ship_small_render(const actor* a)
{
	glPushMatrix();
	glTranslatef(a->pos[0], a->pos[1], a->pos[2]);
	glRotatef(-atan2f(a->vel[0], a->vel[1])*RAD2DEG, 0.0f, 0.0f, 1.0f);
	glRotatef(a->ang, 0.0f, 1.0f, 0.0f);
	glColor4f(0.0f,0.0f,0.0f, 0.2f);
	glScalef(0.2f,0.7f,0.2f);
	//center
	cube_w();
	glPushMatrix();
	glTranslatef(-0.9f,-0.4f, 0.0f);
	glScalef(3.0f,0.6f,1.0f);
	cube_w();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.9f,-0.4f, 0.0f);
	glScalef(3.0f,0.6f,1.0f);
	cube_w();
	glPopMatrix();


	//ship_render();		
	glPopMatrix();
}


///inits fixed position
void init_fixed(enemy_pattern* p)
{
	p->fixed_pos = rand01();
	p->fixed_ang = linear(0.0f,-PI/3.0f, rand01());

}

void create_pattern(enemy_pattern* p)
{
	//sequence
	int seq[] = { FIXED_POS, FIXED_POS, RANDOM_POS };
	p->sequence = RANDOM_POS;//seq[randi(4)];
	//p->sequence = RANDOM_POS;
	//if(p->sequence == FIXED_POS)
	init_fixed(p);
}

#define smoothjump (a, b, tran, t) (smoothstep(a, a + trans, t) * (1.0f - smoothstep(b - trans, b, t)));


void small_update(actor* ac, float dt)
{
	float t = ac->time/10.0f;
	//ac->vel[1] = -30.0f*( smoothstep(0.4f,0.5f, 0.5f*(cosf(t*2.0f*PI))) - 0.5f);
	float x = cosf(t*2.0f*PI);
	float a = 10.0f*x*x*x;
	
	
	//ac->vel[0] = a*cosf(ac->ang);
	//ac->vel[1] = a*sinf(ac->ang);


	ac->ang = ac->aux[0];// + clamp(0.0f, 2*PI, 40.0f*t - 3.0f)* ac->aux[1];
	
	switch(ac->count)
	{
		case 0:
			 ac->ang += clamp(0.0f, 2*PI, 40.0f*t - 3.0f)* ac->aux[1];
			 break;
		case 1:
			///recto;
			break;
		case 2:
		case 3:
			 ac->ang += t* ac->aux[2]*7.0f;


	}
	
	ac->vel[0] = 20.0f*cosf(ac->ang);
	ac->vel[1] = 20.0f*sinf(ac->ang);
	
	MADD(ac->pos, ac->pos, dt, ac->vel);
	//a->ang += 4000.0f*dt;

	// fire

	if(rand01() < 0.01f)
	{
		vec3f v;
		VMOV3(v, ac->vel[0]*rand01(), -20.0f, 0.0f);
		BULLET_shot(ac->pos,v,  BULLET_ENEMY);
	}
}

void medium_update_mobile(actor* ac, float dt)
{
	float t = ac->time/10.0f;
	ac->vel[1] = -3.0f;
	//MADD(ac->pos, ac->pos, dt, ac->vel);
	float inc = 0.3f*cos(10.0f*t);
	ac->pos[1] -= inc;
	if(inc > 0.29f && ac->count == 0)
	{	
		int n = 3 + randi(5);
		for(int j = 0; j < n; ++j)
		{
					vec3f vel;
					float ang = -0.5f*PI - linear(-PI/2.0f, PI/2.0f, float(j)/float(n));
					vel[0] = 15.0f*cosf(ang);
					vel[1] = 15.0f*sinf(ang);
					vel[2] = 0.0f;
					BULLET_random_bullet(ac->pos,vel);
		}
		ac->count = 1;
	}
}

void medium_update(actor* ac, float dt)
{
	float t = ac->time/10.0f;
	ac->vel[1] = -3.0f;
	MADD(ac->pos, ac->pos, dt, ac->vel);

	ac->ang = ac->aux[2]*45.0f + -90.0f - 45.0f*sin(ac->time*0.25f);	

	if(ac->count%15 == 0 && ac->count & 0x80)
	{
		vec3f v;
		//VMOV3(v, ac->vel[0]*rand01(), -20.0f, 0.0f);
		v[0] = 20.0f*cosf(ac->ang*DEG2RAD);
		v[1] = 20.0f*sinf(ac->ang*DEG2RAD);
		v[2] = 0.0f;
		BULLET_shot(ac->pos,v,  BULLET_ENEMY);
	}
	ac->count = ac->count++ & 0xFF;

}

void final_boss_ap(enemy_pattern* ap)
{
	ap->sequence = FIXED_POS;
	ap->side = TOP;
	ap->next_cnt = ap->num_cnt = ap->group_num = 1;
	ap->group_interval = 1;
	ap->interval = 1;
	ap->update_fn = final_boss_update;
	ap->render_fn = final_boss_render;
	ap->ship_type = SHIP_FINAL_BOSS;
}

void small_app(enemy_pattern* ap)
{
	 int sides[] = { SIDE_L, SIDE_R, TOP, TOP };
	 int a = randi(4);
	 ap->sequence = FIXED_POS;
	 ap->side = sides[a];
	 ap->aux[1] = rand01() < 0.5? -1.0f:1.0f; //aux[1] spin direction
	 switch (randi(4)) {
	   case 0:
        ap->group_num = 7 + randi(3);
        ap->group_interval = 100 + randi(15);
        ap->interval = 6 + randi(5);
        break;
      case 1:
		ap->group_num = 5 + randi(3);
        ap->group_interval = 56 + randi(10);
        ap->interval = 6 + randi(5);        
        break;
      case 2:
      case 3:
		ap->group_num = 2 + randi(2);
        ap->group_interval = 45 + randi(20);
        ap->interval = 6 +randi(5);     
       break;
    }

	ap->num_cnt = ap->group_interval;
	ap->next_cnt = ap->group_num;
	ap->update_fn = small_update;
	ap->render_fn = ship_small_render;
	ap->ship_type = SHIP_SMALL;
	ap->count = randi(4);
}

void medium_app(enemy_pattern* ap)
{
	 
	 ap->side = TOP;
	 switch (randi(3)) {
      case 0:       
		ap->group_num = 3 + randi(3);
        ap->group_interval = 256 + randi(10);
        ap->interval = 35 + randi(5);       
		ap->sequence = FIXED_POS;
		ap->update_fn = medium_update;
		ap->render_fn = ship_medium_render;
        break;
      case 1:      
		ap->group_num = 1 + randi(2);
        ap->group_interval = 200 + randi(20);
        ap->interval = 100 + randi(50);    
		ap->update_fn = medium_update;
		ap->render_fn = ship_medium_render;
	  case 2:
		ap->group_num = 1 + randi(2);
        ap->group_interval = 200 + randi(20);
        ap->interval = 45 + randi(50);    
		ap->update_fn = medium_update_mobile;
		ap->render_fn = ship_medium_render_mobile;
		ap->sequence = RANDOM_POS;
       break;
    }

	ap->num_cnt = ap->group_interval;
	ap->next_cnt = ap->group_num;

	ap->ship_type =  SHIP_MEDIUM;
	ap->aux[1] = randf()*95.0f; //start ang
}

void ENEMY_init(int* ship_types)
{
	int i;
	enemy_patterns = 0;
	for(i = 0; i < ship_types[0]; ++i)
	{
		create_pattern(&patterns[enemy_patterns]);
		small_app(&patterns[enemy_patterns]);
		++enemy_patterns;
	}	

	for(i = 0; i < ship_types[1]; ++i)
	{
		create_pattern(&patterns[enemy_patterns]);
		medium_app(&patterns[enemy_patterns]);
		++enemy_patterns;
	}	

	for(i = 0; i < ship_types[2]; ++i)
	{
		pal_change_to(0);
		create_pattern(&patterns[enemy_patterns]);
		final_boss_ap(&patterns[enemy_patterns]);
		++enemy_patterns;
	}	
}




void emit(enemy_pattern* p, float* pos, float d, int type)
{
	actor* a = ACTOR_get(actor_pool);
			
	a->pos[0] = pos[0];
	a->pos[1] = pos[1];
	a->pos[2] = 0.0f;

	a->vel[0] = cosf(d);//randf()*4.0f;
	a->vel[1] = sinf(d);
	a->vel[2] = 0.0f;

	a->ang = 0.0;
	a->aux[0] = d;
	a->collide_size[0] = 1.0f;
	a->collide_size[1] = 1.0f;
	a->aux[1] = p->aux[1];
	a->aux[2] = rand01();

	a->type = type;
	a->update = p->update_fn;
	a->render = p->render_fn;
	if(a->type == SHIP_FINAL_BOSS)  //hack
		final_boss_init(a);

	if(a->type == SHIP_SMALL)
	{
		a->count = p->count;
		
	}

	
	
}


void emit_enemies()
{
	int i;
	for(i = 0; i < enemy_patterns; ++i)
	{
		enemy_pattern* p = &patterns[i];
		p->num_cnt--;
		if(p->num_cnt == 0)
		{
			//emit here
			float emit_pos[3];
			float pos;
			float ang ;
			if(p->sequence == FIXED_POS)
			{
				pos = p->fixed_pos;
			} 
			else
			{
				pos = rand01();
			}

			switch(p->side)
			{
				case TOP:
					emit_pos[0] = (pos - 0.5f)*board_x;
					emit_pos[1] = board_y*0.5f;
					emit_pos[2] = 0.0f;
					ang = -PI/2.0f;
					emit_pos[0]*=0.6f;
					break;
				case SIDE_L:					
				case SIDE_R:
					emit_pos[0] = board_x * (p->side == SIDE_L ? -0.5f: 0.5f);
					emit_pos[1] = linear(0.0f, board_y*0.5f, pos);
					emit_pos[2] = 0.0f;				
					ang = p->fixed_ang +  (p->side == SIDE_L ? PI + PI*0.5f: PI);	
					if(p->side == SIDE_R)
						ang = ang = -p->fixed_ang + PI;
					else 
						ang = ang = p->fixed_ang ;//+ PI*1.5f;
					break;
			}

			
			emit(p, emit_pos, ang, p->ship_type);

			// update counters
			p->next_cnt --;
			if(p->next_cnt == 0)
			{
				// ship group has been emitted, wait for next slot
				create_pattern(p);	
				if(p->ship_type ==  SHIP_MEDIUM)
					medium_app(p);
				else if(p->ship_type ==  SHIP_SMALL)
					small_app(p);
			}
			else
			{
				// there are ships in the group already
				p->num_cnt = p->interval;
			}
			
		}

	}

}

int emiter_cnt = 1;

int level_section = 0;
int level_counter = 0;
int LEVEL_SECTION_CNT = 1000;
int level_current = 0;
#define MAX_LEVEL_SECTION 5
int level[][MAX_LEVEL_SECTION][3] = { 
	{{ 2,0, 1} , { 2, 0, 0 }, { 0, 1, 0 } , { 1, 1, 0 }, { 0, 0, 1 } }, // level
	{{ 1,0, 0} , { 2, 0, 0 }, { 2, 1, 0 } , { 2, 2, 0 }, { 0, 0, 1 } },
	{{ 2,0, 0} , { 2, 1, 0 }, { 3, 1, 0 } , { 2, 3, 0 }, { 0, 0, 1 } },
	{{ 3,1, 0} , { 2, 3, 0 }, { 4, 2, 0 } , { 4, 2, 0 }, { 0, 0, 1 } },
	
};

void LEVEL_update()
{
	if(level_counter-- == 0)
	{
		level_counter = LEVEL_SECTION_CNT;
		if(level_section < MAX_LEVEL_SECTION)
		{
			ENEMY_init(level[level_current][level_section]);
		}
		else 
		{
			//reset patterns
			int empty[] = {0,0,0};
			ENEMY_init(empty);
			
		}
		level_section++;
		
	}
	if(emiter_cnt-- == 0)
	{
		emit_enemies();
		emiter_cnt = 1;
	}
}
extern struct actor_t* final_boss;
void LEVEL_init(int level)
{
  level_section = 0;
  level_counter = 0;
  level_current = level;  
  ACTOR_init(actor_pool);
  final_boss = 0;
  PLAYER_init();
  pal_change_to(1 + randi(2));
  SOUND_play_song(1);
}