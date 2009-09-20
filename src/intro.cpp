//--------------------------------------------------------------------------//
// iq / rgba  .  tiny codes  .  2008                                        //
//--------------------------------------------------------------------------//

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <mmsystem.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "intro.h"
#include "mzk.h"

#include "actor.h"
#include "renderables.h"
#include "player.h"
#include "board.h"
#include "enemy.h"
#include "particle.h"

#include "boss.h"

#include "menu.h"

#include "snd/sound.h"
#include "snd/effects.h"



int game_state = INTRO;
int game_counter = 0;

int input_delay = 0;

void test_calculate_boss()
{	
	int i;
	for(i = 0; i < MAX_ACTOR; ++i)
	{
		actor* a = &actor_pool[i];
		if(a->type == SHIP_FINAL_BOSS)
		{
			final_boss_init(a);
		}
	}


}
void check_collisions()
{
	
	int i;
	int j;
	//remove bullets 
	for(i = 0; i < MAX_ACTOR; ++i)
	{
		actor* a = &actor_pool[i];
		if(!BOARD_inside(a->pos))
			ACTOR_kill(a);
	}

	// bullet vs enemies
	for(i = 0; i < MAX_ACTOR; ++i)
	{
		actor* a = &actor_pool[i];
		if(IS_ACTIVE(a) && 
		  (a->flags & F_BULLET) )
		{
			if( a->flags & F_COLLIDE_ENEMY)
			{
				for(j = 0; j < MAX_ACTOR; ++j)
				{
					actor* b = &actor_pool[j];				
					if(IS_ACTIVE(b) && !(b->flags & F_BULLET))
					{
						if(b->type ==  SHIP_FINAL_BOSS)
						{
								//PART_damage(b->pos);
								if(final_boss_collide(b, a))
								{
									ACTOR_kill(a);
								}
						} else						// player bullets
						if(ACTOR_collide(b, a->pos))
						{
							if( b->type == SHIP_MEDIUM)
							{								
								if(rand01() < 0.01)
								{
									PART_explosion(b->pos, 5);
								} 
								else
								{
									PART_damage(b->pos, 1.0f);
								}
								b->life -= 0.1f; //3 bullets
								if(b->life < 0.0f)
								{
									vec3f v;									
									PART_explosion(b->pos);
									VMOV3(v, randf()*3.0f, randf()*3.0f,0.0f);									
									VADD(v, b->pos, v);
									PART_explosion(v);
									VMOV3(v, randf()*3.0f, randf()*3.0f,0.0f);									
									VADD(v, b->pos, v);
									PART_explosion(b->pos);
									ACTOR_kill(b);
									EFFECTS_medium_explosion();
								}
							}
							else if(b->type ==  SHIP_SMALL)
							{
								ACTOR_kill(b);
								PART_explosion(b->pos);
								EFFECTS_small_explosion();
							}
							
							//kill bullet
							ACTOR_kill(a);
							
							//GARBAGE_explosion(a->pos);
						}
						// enemy bullets
						
					}
				}
			} 
			else if(a->flags & F_COLLIDE_PLAYER)
			{
				if(game_state == INTRO) //hack
					return; 
				vec3f margin;
				VMOV3(margin, 1.5f, 3.0f, 1.0f);
				if(is_inside_2d(pj_pos, margin, a->pos))
				{
					pj_life -= 0.21f;
					PART_explosion(pj_pos);
					ACTOR_kill(a);
				}
			}
		}
	}

}




// --- input 
void input()
{
	if(input_delay < 0)
	{
		if(GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_RETURN))
		{
				MENU_show();
				if(game_state == INTRO)
				{
					
					LEVEL_init(1);
					game_state = GAME;
				}
				else if(game_state == GAME)
				{
					game_state = PAUSE;
				} 
				else 
				{
					game_state = GAME;
				}
				
		}
		input_delay = 10;
	}
	input_delay--;

	
	if(game_state == GAME)
	{
		pj_input[0] = GetAsyncKeyState(VK_UP) ? 1:0;
		pj_input[1] = GetAsyncKeyState(VK_DOWN) ? 1:0;
		pj_input[2] = GetAsyncKeyState(VK_LEFT) ? 1:0;
		pj_input[3] = GetAsyncKeyState(VK_RIGHT) ? 1:0;
		pj_input[4] = GetAsyncKeyState('A') ? 1:0;
		pj_old_powering = pj_powering;
		pj_powering = GetAsyncKeyState('S') ? 1:0;	  

		if(GetAsyncKeyState(VK_MBUTTON))
			test_calculate_boss();

		
	}
}

void update()
{
	
	if(game_state == PAUSE)
		return;
	PLAYER_update();
	LEVEL_update();
	ACTOR_update(actor_pool, dt);	
	ACTOR_update(particle_pool, dt);
	BOARD_update();	
	check_collisions();

	if(game_state == INTRO)
	{
		game_counter++;
		if(game_counter % 500 == 0)
		{
			test_calculate_boss();
		}
	}

}

void render()
{
	glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glTranslatef(0.0f,0.0f, -40.0f);
	glColor3f(1.0f,1.0f,1.0f);

	

	glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
	BOARD_render();
	if(game_state != INTRO)
		PLAYER_render();

	ACTOR_render(actor_pool);
	ACTOR_render(particle_pool);

	
	
}
//----------------------------------------

#define fzn  0.005f
#define fzf  1000.0f
long t0, t1;
void intro_init( void )
{
	static const float projectionmatrix[16] = {
    1.0f, 0.00f,  0.0f,                    0.0f,
    0.0f, 1.25f,  0.0f,                    0.0f,
    0.0f, 0.00f, -(fzf+fzn)/(fzf-fzn),    -1.0f,
    0.0f, 0.00f, -2.0f*fzf*fzn/(fzf-fzn),  0.0f };

    glDisable(GL_DEPTH_TEST);
	glEnable( GL_BLEND );
	glMatrixMode( GL_PROJECTION );
	glLoadMatrixf( projectionmatrix );
    glMatrixMode( GL_MODELVIEW );
//	back_init();
	ACTOR_init(actor_pool);
	cube_init();
	
	BOARD_init();
	
	t0 =  timeGetTime();

	
}



void sound_precache()
{
	EFFECTS_init();
	//SOUND_play_song(0);
}



long DT = 20;
void intro_do( long itime )
{
	t1 = timeGetTime();

	input();
	while((t1 - t0) > DT )
	{

   		
		update();
		MENU_update();		
		
		t0 += DT;
	}
	
	render();
	MENU_render();
	BOARD_hide();
    
}

