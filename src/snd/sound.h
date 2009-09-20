#pragma once
#include "synth.h"

#define USEFMOD

#define PLAY_ONE 0
#define PLAY_LOOP 1

#define MAX_SAMPLE_GENERATOR 32
#define MAX_BUFFERS 32

#ifdef __cplusplus
extern "C" {
#endif

int  SOUND_init();
void SOUND_play_song(int idx);
void SOUND_stop_song();

struct sample_t
{
	struct Channel c;
	int samples;
	int note;
};
typedef struct sample_t sample;

//LPDIRECTSOUNDBUFFER CreateSoundEffect(struct Channel *c, int samples,int note);

void SOUND_add_sample(struct Channel *c, int samples,int note);
int  SOUND_create_sample(int sample_id);
void SOUND_play(int sound,int mode);
void SOUND_stop(int sound);
void SOUND_set_frequency(int sound, int freq);
void SOUND_set_vol(int sound, float vol);

#ifdef __cplusplus
}
#endif




//
// http://msdn.microsoft.com/coding4fun/gamedevelopment/beginning8/default.aspx 