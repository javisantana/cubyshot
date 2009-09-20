#include "font.h"
#include "math.h"
#include <gl/gl.h>
#include <stdio.h>


unsigned int font_base;
//ADAPTACIÓnN
/*

This code has been written by Pan aka Marco *PaN!* Foco of SpinningKids
It's no use to put a copyright on it, since it's so simple... just use it and credit me somehow if you like it.

This is just a portion of code from SpinningKids' intro "PK is dead". 

You can get it from http://www.pouet.net/prod.php?which=5610

Share & enjoy =)

--*PaN!*
pan@spinningkids.org
*/


GLYPHMETRICSFLOAT gmf[96];
void init_font(HDC hDC)
{

   
    HFONT	font = CreateFont(	12,							// Height Of Font
		0,								// Width Of Font
		0,								// Angle Of Escapement
		0,								// Orientation Angle
		900,					// Font Weight
		FALSE,							// Italic
		FALSE,							// Underline
		FALSE,							// Strikeout
		ANSI_CHARSET,					// Character Set Identifier
		OUT_TT_PRECIS,					// Output Precision
		CLIP_DEFAULT_PRECIS,			// Clipping Precision
		ANTIALIASED_QUALITY,			// Output Quality
		FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
		"Arial");				// Font Name

	SelectObject(hDC, font);

	font_base = glGenLists(96);

  wglUseFontOutlines(hDC, 32, 96, font_base,
		0.015f,							// Deviation From The True Outlines
		0.0f,							// Font Thickness In The Z Direction
		WGL_FONT_POLYGONS, gmf);

  DeleteObject(font);

}

void font_printf(float x,float y,float z,float tam ,const char *fmt, ...) 
{
	char text[256];
		float length=0;
	va_list		ap;
	int l = 0;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

    
	while(text[l])l++;
	glPushMatrix();
	glTranslatef(x,y,z);
	glScalef(tam,tam,tam);
	glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	glListBase(font_base-32);									// Sets The Base Character to 0
	glCallLists(l, GL_UNSIGNED_BYTE, text);	// Draws The Display List Text
    glListBase(0);
	glPopAttrib();										// Pops The Display List Bits
	glPopMatrix();			

}

void advancechar(char c,float tracking)
{
	c-=32;
 glTranslatef(gmf[c].gmfCellIncX*(tracking+1)*0.9f, 0, 0);
}
void printChar(char c, float lsx, float lsy, float lr, float tracking) 
{
  c -= 32;

    glPushMatrix();
    glTranslatef(gmf[c].gmfCellIncX/2.0f, gmf[c].gmfBlackBoxY/2.0f, 0);
    glRotatef(lr, 0, 0, 1);
    glScalef(lsx, lsy, 1);
    glTranslatef(-gmf[c].gmfCellIncX/2.0f, -gmf[c].gmfBlackBoxY/2.0f, 0);
    glCallList(font_base+c);
    glPopMatrix();									
   
}



void font_printf_guay(float x,float y,float z,float tam ,const char *fmt, ...) 
{
	char* p;
	char text[256];
		float length=0;
	va_list		ap;
	int l = 0;
	va_start(ap, fmt);
	vsprintf(text, fmt, ap);
	va_end(ap);

    
	while(text[l])l++;
	glPushMatrix();

	glTranslatef(x,y,z);
	glScalef(tam,tam,tam);
	//glPushAttrib(GL_LIST_BIT);							// Pushes The Display List Bits
	//glListBase(font_base-32);									// Sets The Base Character to 0
	
	glEnable(GL_BLEND);
	//
	

	{
		p = text;
		while(*p)
		{
			int v = 2;

			
			glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
			while(v--)
			{
				glColor4f(0.0f,0.0f,0.0f,0.2f);
				//printChar(*p,1.5f+ sfrand()*0.2f,1.5f+sfrand()*0.2f,sfrand()*5,0.5);
				printChar(*p,1.5f,1.5f,rand01()*5,0.5f);
			}
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
			glLineWidth(2.0);
			v = 3;
			
			while(v--)
			{
				//glPushMatrix();
				
				glColor4f(0.7f + randf()*0.3f,0.2f,0.2f,0.3f);
				printChar(*p,1.0f+ randf()*0.1f,1.0f+randf()*0.1f,randf()*5.0f,0.8f);
				//glPopMatrix();
				/*

				glTranslatef(6+random.random()*0.5,0,0)
				glColor4f(self._color1[0],self._color1[2],self._color1[2],self._color1[3])
				self._font.PrintChar(0,0,x,5*(random.random()*2 -1),1.0+random.random()*0.1,self._glDst);
				#glColor4f(0.6,0.6,0.4,0.5)
				self._font.PrintChar(0,0,x,6*(random.random()*2 -1),self._glDst);
				#glColor4f(1.0,0.2,0.2,0.3)
				self._font.PrintChar(0,0,x,5*(random.random()*2 -1),self._glDst);
				glColor4f(self._color2[0],self._color2[2],self._color2[2],self._color2[3])
				self._font.PrintChar(0,0,x,7*(random.random()*2 -1),self._glDst);
				glColor3f(1,1,1)
				*/
				
			}
			

			//glTranslatef(10,0,0);
			advancechar(*p, 0.1f);
			
			p++;
		}
	}
	glPopMatrix();
	glDisable(GL_BLEND);

	//glPolygonMode(GL_FRONT_AND_BACK,GL_POINT);
	//glCallLists(l, GL_UNSIGNED_BYTE, text);
	//glPopMatrix();
	//glPushMatrix();
	//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	//glCallLists(l, GL_UNSIGNED_BYTE, text);
	
	//glCallLists(l, GL_UNSIGNED_BYTE, text);
	/*{
		int v = 3;
		while(v--)
		{
			glPushMatrix();
			glRotatef(sfrand()*2,0,0,1);
			glColor4f(0.0f,0.8f + sfrand()*0.2f,0.0f,0.1f);
			glCallLists(l, GL_UNSIGNED_BYTE, text);
			glPopMatrix();
			
		}

	}*/
	/*
	p = text;
	while(*p)
	{
		int v = 1;
		glColor4f(0.0f,1.0f,0.0f,0.5f);
		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		//glCallLists(1, GL_UNSIGNED_BYTE, p);	// Draws The Display List Text
		glColor4f(0.0f,0.4f,0.0f,0.8f);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		while(v--)
		{
			glPushMatrix();
			glRotatef(sfrand(),0,0,1);
			glCallLists(1, GL_UNSIGNED_BYTE, p);
			//glPopMatrix();

		}

		p++;
	}*/
	
   // glListBase(0);
	//glPopAttrib();										// Pops The Display List Bits
		

}
