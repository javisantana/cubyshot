#pragma once

extern int pj_input[5];
extern int pj_powering;
extern int pj_old_powering;
extern float pj_pos[2];
extern float pj_vel[2];
extern float pj_life;

void PLAYER_render();
void PLAYER_update();
void PLAYER_init();