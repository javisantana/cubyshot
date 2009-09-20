//SOUND_add_sample(&canal,44100*2,12);

#include "sound.h"
/*
struct Channel ch3 =       {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,      { NO_OSC,24,1,1  ,0,2100,-0.05f,0.3f,5e-006,0.003f,1,44100}};
struct Channel canal = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f, { NO_OSC,24,1,0.5,0,1200,0,0.9,0,0,0.6,44100}};
struct Channel turbo = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,{ SIN,24,1,1.2,-0.000132,1500,-0.06,0.5,7e-006,-0.017,0.6,13500}};
*/
//struct Channel turbo2 = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f, { SIN,12,1,1.3f,-4.1e-005f,2700,0,0.7f,3e-006f,-0.002f,0.2f,44100}};//freq: 65.4064
struct Channel player_shot =       { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f, { SIN,17,1,0.201f,-0.000122f,3000,0,0.7f,-4e-006f,0.033f  ,1.0f,2100}};//freq: 87.3071
struct Channel small_explode = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,{  SQUARE,1 ,1,0.3f  ,-0.000011f,1000,0,0.0f,0.0f    ,-0.0011f,2.0f,22900}};//freq: 34.6478
struct Channel medium_explode = { 0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,{ SQUARE,1,1,0.6f,-1.3e-005f,1000,0,0.3f,0,-0.0011f,2,22900}};

int SND_efect_player_shoot = -1;

int SND_efect_small_explode[3];
int SND_efect_small_explode_cnt = 0;

int SND_efect_medium_explode[3];
int SND_efect_medium_explode_cnt = 0;

void EFFECTS_init()
{
	SOUND_add_sample(&player_shot,2100,12);	
	SND_efect_player_shoot = SOUND_create_sample(0);

	SOUND_add_sample(&small_explode,22900,12);
	SND_efect_small_explode[0] = SOUND_create_sample(1);
	SND_efect_small_explode[1] = SOUND_create_sample(1);
	SND_efect_small_explode[2] = SOUND_create_sample(1);

	SOUND_add_sample(&medium_explode,30000,18);
	SND_efect_medium_explode[0] = SOUND_create_sample(2);
	SND_efect_medium_explode[1] = SOUND_create_sample(2);
	SND_efect_medium_explode[2] = SOUND_create_sample(2);
	
}

 

void EFFECTS_player_shoot()
{

	SOUND_play(SND_efect_player_shoot, PLAY_ONE);
}

void EFFECTS_small_explosion()
{
	
	SOUND_play(SND_efect_small_explode[SND_efect_small_explode_cnt], PLAY_ONE);
	SND_efect_small_explode_cnt= SND_efect_small_explode_cnt++%3;
}

void EFFECTS_medium_explosion()
{
	
	SOUND_play(SND_efect_medium_explode[SND_efect_medium_explode_cnt], PLAY_ONE);
	SND_efect_medium_explode_cnt= SND_efect_medium_explode_cnt++%3;
}
