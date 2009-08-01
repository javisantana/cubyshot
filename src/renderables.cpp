
#include "renderables.h"
//------------------ TEXTURE

#include "math.h"

#include <windows.h>
#include <gl/gl.h>




void DrawCube2(float x,float y,float z)
{
	
	x/=2;
	y/=2;
	z/=2;
	

	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f*x, -1.0f*y,  1.0f*z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f*x, -1.0f*y,  1.0f*z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f*x,  1.0f*y,  1.0f*z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f*x,  1.0f*y,  1.0f*z);
		// Back Face
		glNormal3f( 0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f*x, -1.0f*y, -1.0f*z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f*x,  1.0f*y, -1.0f*z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f*x,  1.0f*y, -1.0f*z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f*x, -1.0f*y, -1.0f*z);
		// Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f*x,  1.0f*y, -1.0f*z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f*x,  1.0f*y,  1.0f*z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f*x,  1.0f*y,  1.0f*z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f*x,  1.0f*y, -1.0f*z);
		// Bottom Face
		glNormal3f( 0.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f*x, -1.0f*y, -1.0f*z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f*x, -1.0f*y, -1.0f*z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f*x, -1.0f*y,  1.0f*z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f*x, -1.0f*y,  1.0f*z);
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f*x, -1.0f*y, -1.0f*z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f*x,  1.0f*y, -1.0f*z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f*x,  1.0f*y,  1.0f*z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f*x, -1.0f*y,  1.0f*z);
		// Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f*x, -1.0f*y, -1.0f*z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f*x, -1.0f*y,  1.0f*z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f*x,  1.0f*y,  1.0f*z);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f*x,  1.0f*y, -1.0f*z);
	glEnd();
}

static int cube_list = -1;
void cube_init()
{
	if(cube_list == -1)
	{
		cube_list = glGenLists(1);
        glNewList(cube_list,GL_COMPILE);
         
        glBegin(GL_QUADS);

        glVertex3f( 1.0f, 1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);		
        glVertex3f(-1.0f, 1.0f, 1.0f);		
        glVertex3f( 1.0f, 1.0f, 1.0f);		

        glVertex3f( 1.0f,-1.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);		
        glVertex3f(-1.0f,-1.0f,-1.0f);		
        glVertex3f( 1.0f,-1.0f,-1.0f);		

        glVertex3f( 1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);		
        glVertex3f(-1.0f,-1.0f, 1.0f);		
        glVertex3f( 1.0f,-1.0f, 1.0f);		

        glVertex3f( 1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);		
        glVertex3f( 1.0f, 1.0f,-1.0f);		

        glVertex3f(-1.0f, 1.0f, 1.0f);
        glVertex3f(-1.0f, 1.0f,-1.0f);		
        glVertex3f(-1.0f,-1.0f,-1.0f);		
        glVertex3f(-1.0f,-1.0f, 1.0f);		

        glVertex3f( 1.0f, 1.0f,-1.0f);
        glVertex3f( 1.0f, 1.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);		
        glVertex3f( 1.0f,-1.0f,-1.0f);		
        glEnd();		
        glEndList();
	}
	//glCallList(cube_list);
}
void cube_w()
{
	glCallList(cube_list);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);	
    glCallList(cube_list);	
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void cube()
{
	glCallList(cube_list);
}

//---------------------------

void quad()
{
	glBegin(GL_QUADS);
			glTexCoord2f(0,0);
			//glVertex3f(-1,-1,0);
			glVertex3f(-1.0f/2.0f,-1.0f/2.0f,0);
			glTexCoord2f(0,1);
			//glVertex3f(-1,1,0);
			glVertex3f(-1.f/2,1.f/2,0);
			glTexCoord2f(1,1);
			//glVertex3f(1,1,0);
			glVertex3f(1.f/2,1.f/2,0);
			glTexCoord2f(1,0);
			//glVertex3f(1,-1,0);
			glVertex3f(1.f/2,-1.f/2,0);
		 glEnd();
}



//render between 0-1.0f
void render_doids(int _seed)
{
	int old_seed = seed;
	seed = _seed;
	{
	int nboxes = 1 + randi(3);
	//random size
	float w = 0.1f + 0.5f*rand01();
	float h = (0.7f - w) + 0.5f* rand01();
	float hg = 0.01f + 0.2f*rand01();

	//pos
	float x = (1.0f - w) * rand01();
	float y = (1.0f - h*nboxes) * rand01();

	while(nboxes--)
	{
		glPushMatrix();
		glTranslatef(x, y + (h+ 0.01f)*nboxes, hg);
		glScalef(w, h, hg);
		cube();
		glPopMatrix();
		
	}
	}

	seed = old_seed;

	
}



/*
void BB_begin()
{

	float modelview[16];
	int i,j;

	// save the current modelview matrix
	glPushMatrix();

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	for( i=0; i<3; i++ ) 
	    for( j=0; j<3; j++ ) {
		if ( i==j )
		    modelview[i*4+j] = 1.0;
		else
		    modelview[i*4+j] = 0.0;
	    }

	// set the modelview matrix
	glLoadMatrixf(modelview);
	

}
void BB_end()
{
	glPopMatrix();
}
*/
//------------------- NOISE

#if 0

glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 6*sizeof(float), cubeVerts+0);
    glNormalPointer(    GL_FLOAT, 6*sizeof(float), cubeVerts+3);
	glDrawElements( GL_QUADS, 6*4, GL_UNSIGNED_INT, cubeIndices );
	glDisableClientState( GL_NORMAL_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );


#define fzn  0.005f
#define fzf  1000.0f

static const float projectionmatrix[16] = {
    1.0f, 0.00f,  0.0f,                    0.0f,
    0.0f, 1.25f,  0.0f,                    0.0f,
    0.0f, 0.00f, -(fzf+fzn)/(fzf-fzn),    -1.0f,
    0.0f, 0.00f, -2.0f*fzf*fzn/(fzf-fzn),  0.0f };

static const float cubeVerts[] = { 

  1.0f, -1.0f, -1.0f,    0.0f,-1.0f, 0.0f,
  1.0f, -1.0f,  1.0f,    0.0f,-1.0f, 0.0f,
 -1.0f, -1.0f,  1.0f,    0.0f,-1.0f, 0.0f,
 -1.0f, -1.0f, -1.0f,    0.0f,-1.0f, 0.0f,

 -1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,
 -1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
  1.0f,  1.0f,  1.0f,    0.0f, 1.0f, 0.0f,
  1.0f,  1.0f, -1.0f,    0.0f, 1.0f, 0.0f,

 -1.0f, -1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,
 -1.0f, -1.0f,  1.0f,   -1.0f, 0.0f, 0.0f,
 -1.0f,  1.0f,  1.0f,   -1.0f, 0.0f, 0.0f,
 -1.0f,  1.0f, -1.0f,   -1.0f, 0.0f, 0.0f,

  1.0f,  1.0f, -1.0f,    1.0f, 0.0f, 0.0f,
  1.0f,  1.0f,  1.0f,    1.0f, 0.0f, 0.0f,
  1.0f, -1.0f,  1.0f,    1.0f, 0.0f, 0.0f,
  1.0f, -1.0f, -1.0f,    1.0f, 0.0f, 0.0f,

 -1.0f, -1.0f, -1.0f,    0.0f, 0.0f,-1.0f,
 -1.0f,  1.0f, -1.0f,    0.0f, 0.0f,-1.0f,
  1.0f,  1.0f, -1.0f,    0.0f, 0.0f,-1.0f,
  1.0f, -1.0f, -1.0f,    0.0f, 0.0f,-1.0f,

  1.0f, -1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
  1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
 -1.0f,  1.0f,  1.0f,    0.0f, 0.0f, 1.0f,
 -1.0f, -1.0f,  1.0f,    0.0f, 0.0f, 1.0f };

static const int cubeIndices[] = { 
 0, 1, 2, 3,
 4, 5, 6, 7,
 8, 9,10,11,
12,13,14,15,
16,17,18,19,
20,21,22,23 };
#endif