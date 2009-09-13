#include "math.h"
#include "actor.h"
//#include <math.h>
#include "enemy.h"
#include "board.h"
#include "bullet.h"
#include "renderables.h"

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

	float aux[3];
	int ship_type;


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


	ac->ang = ac->aux[0] + clamp(0, 2*PI, 40.0f*t - 3.0f)* ac->aux[1];
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



void medium_update(actor* ac, float dt)
{
	float t = ac->time/10.0f;
	//ac->vel[1] = -30.0f*( smoothstep(0.4f,0.5f, 0.5f*(cosf(t*2.0f*PI))) - 0.5f);
	/*float x = cosf(t*2.0f*PI);
	float a = 10.0f*x*x*x;
		
	ac->vel[0] = a*cosf(ac->ang);
	ac->vel[1] = a*sinf(ac->ang);
	*/
	ac->vel[1] = -10.0f;
	MADD(ac->pos, ac->pos, dt, ac->vel);
	//a->ang += 4000.0f*dt;
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
	ap->ship_type = SHIP_SMALL;
}

void medium_app(enemy_pattern* ap)
{
	 
	 ap->side = TOP;
	 switch (randi(4)) {
      case 0:
        ap->group_num = 3 + randi(3);
        ap->group_interval = 72 + randi(15);
        ap->interval = 35 +randi(5);
        break;
      case 1:
		ap->group_num = 1 + randi(2);
        ap->group_interval = 56 + randi(10);
        ap->interval = 30 + randi(5);        
        break;
      case 2:
      case 3:
		ap->group_num = 2 + randi(2);
        ap->group_interval = 45 + randi(20);
        ap->interval = 35 + randi(5);     
       break;
    }

	ap->num_cnt = ap->group_interval;
	ap->next_cnt = ap->group_num;
	ap->update_fn = medium_update;

	ap->ship_type =  SHIP_MEDIUM;
}

void ENEMY_init(int* ship_types)
{
	int i;
	enemy_patterns = 0;
	for(i = 0; i < ship_types[0]; ++i)
	{
		create_pattern(&patterns[i]);
		small_app(&patterns[i]);
		++enemy_patterns;
	}	

	for(i = 0; i < ship_types[1]; ++i)
	{
		create_pattern(&patterns[i]);
		medium_app(&patterns[i]);
		++enemy_patterns;
	}	

	for(i = 0; i < ship_types[2]; ++i)
	{
		create_pattern(&patterns[i]);
		small_app(&patterns[i]);
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

	a->type = type;
	a->update = p->update_fn;
	a->render = ship_small_render;

	
	
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
				else
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
int LEVEL_SECTION_CNT = 2000;
int level[][3] = { 
	{ 1,1, 0 } , { 2, 0, 0 }, { 1, 1, 0 } , { 2, 2, 0 }, { 5, 3, 0 }
};

void ENEMY_update()
{
	if(level_counter-- == 0)
	{
		ENEMY_init(level[level_section]);
		level_section++;
		level_counter = LEVEL_SECTION_CNT;
	}
	if(emiter_cnt-- == 0)
	{
		emit_enemies();
		emiter_cnt = 1;
	}
}