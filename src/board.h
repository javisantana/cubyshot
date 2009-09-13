#pragma once 


const float BOARD_margin = 0.01f;
const float board_x = 30.0f;
const float board_y = 50.0f;

///return if point is inside board
int BOARD_inside(float* p);
void BOARD_update();
void BOARD_render();
void BOARD_init();