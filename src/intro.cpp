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
#include "player.h"
#include "board.h"
#include "enemy.h"

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
	PLAYER_update();
	ENEMY_update();
	ACTOR_update(actor_pool, dt);	
}

void render()
{
	glClearColor( 1.0f, 1.0f, 1.0f, 0.0f );
	glClear( GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	glTranslatef(0.0f,0.0f, -40.0f);
	glColor3f(1.0f,1.0f,1.0f);

	BOARD_render();

	glEnable (GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glColor4f(0.0f, 0.0f, 0.0f, 0.1f);
	PLAYER_render();

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
	glEnable( GL_BLEND );
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

