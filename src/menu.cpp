#include "renderables.h"
#include "board.h"
#include "font.h"
#include "menu.h"

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

	font_printf_guay(-board_x,board_y*.5f,-4.f,2.3f,"points");
	
	glPopMatrix();


}