#include "math.h"
#include "board.h"
#include "renderables.h"
#include "palete.h"

#include "city.h"

vec3f board_pos;
int board_seed = 1;

int BOARD_inside(float* p)
{
	return p[0] >= -board_x*0.5f &&
		   p[0] <= board_x*0.5f &&
		   p[1] >= -board_y*0.5f &&
		   p[1] <= board_y*0.5f;

}

void BOARD_init()
{
	VMOV3(board_pos, 0.0f, 0.0f, 0.0f);
	CITY_init(1);
}
void BOARD_update()
{
	board_pos[0] -= 0.14f;
	board_pos[1] -= 0.04f;
	board_pos[2] -= 0.010f;
	
	board_pos[0] = -board_y*mod(board_y, -board_pos[0]);
	board_pos[1] = -board_y*mod(board_y, -board_pos[1]);
	board_pos[1] = -board_y*mod(board_y, -board_pos[1]);

	//CITY_update(0.01f);
}

#include "city.h"

void CLOUDS_render(int seed, float ss, int rows = 50)
{
	
	for(int j = 0; j < rows; ++j)
	{
		for(int i = 0; i < 25; ++i)
		{
			glPushMatrix();
			float s = fbm(float(i + seed)*0.2f, float(j - seed)*0.2f);
			if(s > 0.5f)
			{
				//s = smoothstep(0.4f, 0.8f, s)*ss + 0.2f;
				//s*=ss;
				s -= 0.5f;
				s*=4.0f;
				glTranslatef(float(i)*0.5f, float(j)*0.5f, 0.0);
				glScalef(s, s, 1.0f);
				//glRotatef(s*180.0f, 0.0f, 0.0f, 1.0f);
				quad_w();
			}


		//	s += 1.0f;
			//s*=0.5f;
//			const float ss = 0.75f;
			/*s = s*step(ss, s) - ss; // 0.65 -> 1.0f
			s/=(1.0f - ss);

			
			if(s > 0.0f)
			{
				//s*=0.5f;
			//	s=s*s;
				glTranslatef(float(i)*0.5f, float(j)*0.5f, 0.0);
				glScalef(s, s, 1.0f);
				quad_w();
			}*/
			glPopMatrix();
		}
	}

}

void BOARD_hide()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLoadIdentity();	
	glTranslatef(0.0f,0.0f, -40.0f);
	//left
	glPushMatrix();	
	glTranslatef(-board_x, 0.0f, 0.0f);
	glScalef(board_x, board_y, 1.0f);
	glColor3f(1.0f,1.0f,1.0f);
	quad();		
	glPopMatrix();
	//r
	glPushMatrix();	
	glTranslatef(board_x, 0.0f, 0.0f);
	glScalef(board_x, board_y, 1.0f);
	glColor3f(1.0f,1.0f,1.0f);	
	quad();		
	glPopMatrix();
	//up
	glPushMatrix();	
	glTranslatef(0.0f, board_y, 0.0f);
	glScalef(board_x*3.0f, board_y, 1.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	quad();		
	glPopMatrix();

	//down
	glPushMatrix();	
	glTranslatef(0.0f, -board_y, 0.0f);
	glScalef(board_x*3.0f, board_y, 1.0f);
	glColor3f(1.0f,1.0f,1.0f);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	quad();		
	glPopMatrix();
}
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

	//draw line border
	glPushMatrix();
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glScalef(board_x, board_y, 1.0f);
	glColor3f(0.0f,0.0f,0.0f);
	quad();
	
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPopMatrix();
	
	int old_seed = seed;
	seed = 6;


	set_pal_color(3, 0.3f);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-6.0f, 0.25f*board_y + board_pos[0], -15.0f);
	CLOUDS_render(3, 0.5f, 60);
	glPopMatrix();

	
	
	set_pal_color(1, 0.05f);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-6.0f, 0.25f*board_y + board_pos[1], -15.0f);
	CLOUDS_render(10, 0.8f);
	glPopMatrix();
	

	
	set_pal_color(0, 0.03f);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-6.0f, 0.25f*board_y + board_pos[2], -15.0f);
	CLOUDS_render(-10, 1.7f);
	glPopMatrix();
	
	


	seed = old_seed;

	


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


	return ;
#if 0


	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-8.0f, board_pos[1], -3.0f);
	glScalef(0.3f,0.3f,1.0f);
	CITY_render();
	glPopMatrix();

	
	glColor4f(0.0f,0.0f,0.0f, 0.05f);

	int old_seed = seed;
	
	for(int j=0; j < 2; ++j)
	{
		seed = board_seed + 34 + j;
		glPushMatrix();
		glTranslatef(0.0f, board_pos[1] + board_y*j, 0.0f);
		for(int i = 0; i < 100; ++i)
		{
				
				glPushMatrix();
				
				glTranslatef(randf()*board_x*0.5f, randf()*board_y*0.5f, 0.0f);
				//cube_w();
				render_doids(randi(300));
				glPopMatrix();
		}
		glPopMatrix();
	}
	 
			/*glPushMatrix();
			glTranslatef(0.0f, board_pos[1] + board_y, 0.0f);
			glTranslatef(randf()*board_x*0.5f, randf()*board_y*0.5f, 0.0f);
			render_doids(randi(300));
			glPopMatrix();
			*/

		
	
	seed = old_seed;
#endif

	
}