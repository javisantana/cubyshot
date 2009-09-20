#pragma once

extern int pj_input[5];
extern int pj_powering;
extern int pj_old_powering;
extern float pj_pos[2];
extern float pj_vel[2];
extern float pj_life;
extern int pj_fire_count;
extern int pj_score;

void PLAYER_render();
void PLAYER_update();
void PLAYER_init();
void PLAYER_kill();