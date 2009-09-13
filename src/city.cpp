
#include "math.h"
#include "renderables.h"


#define HM_X 16
#define HM_Y 64

float hm[HM_X*HM_Y];
#define numPoints 50
#define X  HM_X
#define Y  HM_Y
float distBuffer[X*Y];
float angles[X*Y];
float xcoords[numPoints];
float ycoords[numPoints];


void test_tex()
{
	int i,j;
	//cellular

	float mindist = 100000;
	float maxdist = 0;
	
	// generate random points
	for(i = 0; i < numPoints;++i)
	{
		xcoords[i] = (float)randi(X);
		ycoords[i] = (float)randi(Y);
	}
	
	//generate cellular
	for(i = 0; i< X; ++i)
	{
	 for(j = 0; j< Y; ++j)
	 {
		 int k;
		 float mind = 10000.0f;
		 float mind2 = 10000.0f;
		 int mink = -1;
		 int idx = j*X + i;

		 for(k = 0; k < numPoints; ++k)
		 {
			 float d =  fabsf(xcoords[k] - i) + fabsf(ycoords[k] - j);
			 if(d < mind2 )
			 {
				 mind2 = d;
				 if(d < mind)
				 {
					 mind2 = mind;
					 mind = d;
				 }
			 }		 
		 }
		 
		 distBuffer[idx] = fabsf(mind2 - mind);
		 mindist = min(mindist, mind);
		 maxdist = max(maxdist, mind);
	 }
	}

	//normalize
	for(i = 0; i< X; ++i)
	{
	 for(j = 0; j< Y; ++j)
	 {
		 int idx = j*X + i;
		 distBuffer[idx] = (distBuffer[idx] - mindist)/(maxdist - mindist);
	 }
	}
   
	//write to texture (debug only)

	/*for(i = 0; i< X; ++i)
	{
	 for(j = 0; j< Y; ++j)
	 {
	  int idx = j*X + i;
	  col = &(img[idx*4]);
	  
	  col[0] = col[1] = col[2] = col[3] = (unsigned char)(255*distBuffer[idx]);
	  for(k = 0; k < numPoints;++k)
	  {
		 if(xcoords[k] == i &&
		   ycoords[k] == j)
		 {
			 col[1] = col[2] = 0;
			 col[0] = 255;
		 }
	  }

	 }
	}

	save_tga(img, X, Y, "perlin1.tga");
	*/
	
	//generate roads map
	for(i = 0; i< X; ++i)
	{
	 for(j = 0; j< Y; ++j)
	 {
	  int min_d = 100000;
	  int idx = j*X + i;
	  
	  //col = &(img[idx*4]);		

	  angles[idx] = -1.0f;
	  angles[idx] = step(0.08f,distBuffer[idx]);

	//  col[0] = col[1] = col[2] = col[3] = (unsigned char)(255*angles[idx]);
	 }
	}
			
	 
	  /*for(k = 0; k < 8; ++k)
	  {			 
		  int idx2 = (j + dirs[k][1])*X + (i + dirs[k][0]);
		  float d = step(0.02f,distBuffer[idx2]) -  step(0.02f,distBuffer[idx]);
		  if(d < min_d )
		  {
			 min_d =  d;		
			 angle = atan2f(dirs[k][1], dirs[k][0]);
		  }
	  }*/


//	save_tga(img, X, Y, "perlin_d.tga");

}

void CITY_init(int seed)
{
}
void CITY_update(float dt)
{
	int i,j;
	if(GetAsyncKeyState(VK_MBUTTON))	
	{
		seed++;
		test_tex();

		for( i = 0; i < HM_X; ++i)
		{
			for( j = 0; j < HM_Y; ++j)
			{
				int idx = j*HM_X + i;
				float f = distBuffer[idx];
				hm[idx] = step(0.07f,f)*f*powf(f + 1.1f*rand01(),2);
				
			}
		}

	//	ACTOR_init(actor_pool);
		//lauch doids
		/*for(i = 0; i < MAX_ACTOR>>1;++i)
		{
			actor* a = ACTOR_get();
			if(a)
			{
				int xx, yy, idx;
				do 
				{	
					xx = 10 + randi(X-15);
					yy = 10 + randi(Y-15);
					idx = yy*HM_X + xx;
				}				
				while(angles[idx] > 0.5f);
				


				a->pos[0] = (float)xx;//xcoords[rpos];
				a->pos[1] = (float)yy;//ycoords[rpos];
				a->pos[2] = 0.1f;		
				a->ang = 2*PI* 0.125f*(float)randi(16);
				a->collide_size[0] = randi(2) == 0? -1.0f: 1.0f;
				a->collide_size[1] = 1.2f + rand01();
				a->update = doid_update;
			}
		}*/
		
	}

	/*
	intro_cam_pos[0] += 0.1f*GetAsyncKeyState(VK_UP) ? 1:0;
	intro_cam_pos[0] -= 0.1f*GetAsyncKeyState(VK_DOWN) ? 1:0;

	intro_cam_pos[1] += 0.1f*GetAsyncKeyState(VK_LEFT) ? 1:0;
	intro_cam_pos[1] -= 0.1f*GetAsyncKeyState(VK_RIGHT) ? 1:0;

	intro_cam_pos[2] += 0.1f*GetAsyncKeyState('Q') ? 1:0;
	intro_cam_pos[2] -= 0.1f*GetAsyncKeyState('A') ? 1:0;
	*/
	//ACTOR_update(actor_pool, dt);
	
}



void CITY_render()
{
	int i,j;
	glEnable(GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glColor4f(0.0f, 0.0f, 0.0f, 0.2f);
		
	for( i = 0; i < HM_X; ++i)
	{
		for( j = 0; j < HM_Y; ++j)
		{
			float s = hm[j* HM_X + i];
			if(s > 0.0f)
			{
				glPushMatrix();
				{
					glTranslatef((float)i, (float)j, s);
					glPushMatrix();
					{
						glScalef(0.4f, 0.4f, s);
						//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);						
						//glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
						//cube();
						glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
						//glColor4f(0.0f, 0.0f, 0.0f,3.0f - s);
						glScalef(1.01f, 1.01f, 1.01f);
						cube();
					}
					glPopMatrix();
					glTranslatef(0.0f, 0.0f, s);
					glScalef(0.3f, 0.3f, 0.3f);
					render_doids(i*j + 1);
				}
				glPopMatrix();
			}
		}
	}

	
	glDisable(GL_DEPTH_TEST);
	
}
