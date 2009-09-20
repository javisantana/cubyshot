#include "renderables.h"
#include "board.h"
#include "font.h"
#include "menu.h"
#include "actor.h"

extern int game_state;
float menu_x = board_x*0.75f;
float menu_target_x = 0.0f;

void MENU_show()
{
	if(game_state == GAME)
	{
		menu_target_x = 0.0f;
	}
	else
	{
		menu_target_x = board_x*.75f;				
		
	}

	
}
void MENU_update()
{
	menu_x += (menu_target_x - menu_x)*0.1f;
}

extern struct actor_t* final_boss;
extern int pj_score;
void MENU_render()
{


	glPushMatrix();	
	glLoadIdentity();
		
	glTranslatef(0.0f,0.0f, -40.0f);

	glPushMatrix();	
	glColor4f(0.0f, 0.0f, 0.0f, 0.5f);
	glTranslatef(-menu_x-board_x*0.125f, 0.0f,  0.0f);
	glScalef(board_x*0.75f, board_y, 1.0f);
	quad();	
	glPopMatrix();
	
	if(game_state == INTRO)
	{
		font_printf_guay(-menu_x-board_x*0.14f - 10.0f,3.3f,-4.f,4.3f,"Cubyshot");
		font_printf_guay(-menu_x-board_x*0.14f - 10.0f,0.8f,-4.f,1.8f,"press fire button to play!");
	}
	else if(game_state == DEAD)
	{
		font_printf_guay(-menu_x-board_x*0.14f - 10.0f,3.3f,-4.f,4.3f,"you are dead");
		font_printf_guay(-menu_x-board_x*0.14f - 10.0f,0.8f,-4.f,1.8f,"press fire button to restart!");
		font_printf_guay(-menu_x-board_x*0.14f - 10.0f,-1.8f,-4.f,1.8f,"your score: %d",pj_score);

		
	}
	else if(game_state == PAUSE)
	{
		font_printf_guay(-menu_x-board_x*0.14f - 10.0f,3.3f,-4.f,4.3f,"Pause");
	}
	else if(game_state == INTER_LEVEL)
	{
		font_printf_guay(-menu_x-board_x*0.14f - 10.0f,3.3f,-4.f,4.3f,"LEVEL");
		font_printf_guay(-menu_x-board_x*0.14f - 10.0f,-1.3f,-4.f,4.3f,"COMPLETED");
		font_printf_guay(-menu_x-board_x*0.14f - 10.0f,-5.8f,-4.f,1.8f,"press fire button to next level");
	
	}

	font_printf_guay(-0.5f*board_x,-board_y*.5f,-4.f,2.3f,"score: %d", pj_score);

	if(final_boss != 0)
	{
		//boss life bar
		glPushMatrix();
		float life_max = board_x*0.75f;
		float life = final_boss->life*life_max;
		glTranslatef(life*0.5f - board_x*0.40f, board_y*0.45f,  0.0f);
		glScalef(life*0.5f, 0.2f, 0.2f);
		cube_w();
		glPopMatrix();

	}
	
	glPopMatrix();


}