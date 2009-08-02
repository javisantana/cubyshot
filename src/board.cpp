#include "board.h"
#include "renderables.h"

int BOARD_inside(float* p)
{
	return p[0] >= -board_x*0.5f &&
		   p[0] <= board_x*0.5f &&
		   p[1] >= -board_y*0.5f &&
		   p[1] <= board_y*0.5f;

}

void BOARD_update(){}
void BOARD_render()
{
	/*glEnable(GL_TEXTURE_2D);
	
	glDisable(GL_DEPTH_TEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glBindTexture(GL_TEXTURE_2D, back_text[1]);	
	glScalef(10.0f, 1.5f, 1.0f);
	quad();
	glBindTexture(GL_TEXTURE_2D, back_text[1]);	
	glScalef(1.5f, 10.0f, 1.0f);
	
	quad();

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	*/

	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glScalef(board_x, board_y, 1.0f);
	glColor3f(0.0f,0.0f,0.0f);
	quad();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();

	
}