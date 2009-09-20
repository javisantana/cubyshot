#pragma once
#include <windows.h>

void init_font(HDC hDC);
void font_printf(float x,float y,float z,float tam ,const char *fmt, ...) ;
void font_printf_guay(float x,float y,float z,float tam ,const char *fmt, ...) ;
