#pragma once






#define clamp(x, a, b) ((x) < (a) ? (a) : (x) > (b) ? (b) : (x))

typedef unsigned char uint8;
typedef char int8;
typedef unsigned short uint16;
typedef short int16;
typedef unsigned int uint32;
typedef int int32;


enum { SQUARE = 0,SIN ,SAW ,NO_OSC };

struct ChannelData
{

	
	char  osc;

	int sample_freq;	
	float vol;

	float env;
	float denv;
//	float ddenv;

	float cut;
	float dcut;

	float reso;
	float dreso;

	float dfrq;

	float noise;
	int   num_samples;


};

struct Channel
{
	
	float pos;
	float speed; 
	float phase;
	float delta;
	float frequency;
	float env;
	//float denv;
	float cut;
	float reso;

	struct ChannelData data;
};

void synth_effect(int16* wav,struct Channel* c,int note, int samples);
