
#include "synth.h"
#include "math.h"

//#define EXPORT_TO_WAV
#ifdef EXPORT_TO_WAV
#include <stdio.h>
#endif
#include <windows.h>

#define PI 3.141516f
#define TWOPI (2.0f*PI)
#define clamp(x, a, b) ((x) < (a) ? (a) : (x) > (b) ? (b) : (x))


#define  samples_per_note  5500

#define  rows_per_pattern 64

#define  samples_per_pattern (rows_per_pattern*samples_per_note)

#define  sample_rate  (44100.0f)
#define  N_SAMPLES (N_PATTERNS_TUNE*samples_per_pattern)

#define NOTE_BASE_SAMPLE 40


#if 1
/*
struct wavh
{
  char riff[4];
  uint32 size;
  char wave[4];
  char fmt[4];
  uint32 num;
  uint16 format;
  uint16 channels;
  uint32 freq;
  uint32 abps;
  uint16 blockalign;
  uint16 bitdepth;
  char data[4];
  uint32 datasize;
  
} 

*/

/*
struct wavh wave = {
  { 'R', 'I', 'F', 'F' },
  sizeof(wave) + N_SAMPLES*2,
  { 'W', 'A', 'V', 'E' },
  { 'f', 'm', 't', ' ' },
  0x10,
  1,
  1,
  44100,
  88200,
  2,
  16,
  { 'd', 'a', 't', 'a' },
  N_SAMPLES*2
};
*/
//extern float Gtime;








void synth_effect(int16* wav,struct Channel* c,int note, int samples)
{
			int i;
			short *buf =wav;
			/*+sizeof(wave);	
			wave.datasize = 44100*2;
			wave.size = sizeof(struct wavh) + 44100*2;
			*(struct wavh*)wav = wave;
			*/

			for(i=0;i<samples;i++)
			{
				float out = 0;
				float r;
					
				if(note != -1)
				{
					c->frequency = (powf(2.0f, (note  - 45.0f - 60.0f +  c->data.sample_freq) / 12.0f) * 440.0f );
					c->env = c->data.env;
					c->cut = c->data.cut;
					c->reso = c->data.reso;
				}
				//if(note == 0) c->env = 0;
				c->delta = c->frequency/sample_rate;
							
			
				c->delta = (c->frequency+=c->data.dfrq)/sample_rate;

				c->env += c->data.denv;
				//HACK
				if(c->env<0)c-> env =0;
			

				c->phase= fmodf(c->phase + c->delta,1.0f);

				//osc

				r = 0.0f;
				switch(c->data.osc)
				{
				
					case SQUARE:	r =  c->phase < 0.5f ? -1.0f : 1.0f;break;
					case SIN:	    r = sinf(2.0f*PI* c->phase);break;
					case SAW:       r =  2.0f * c->phase - 1.0f;
				}
				r = r + c->data.noise* (((rand() & 1023) - 511) / 511.0f);
				// ~osc

				//filter
				c->cut+=c->data.dcut;
				c->reso+=c->data.dreso;

				c->speed = c->speed + (r - c->pos) * (2.0f - 2.0f*cosf(2.0f*PI*c->cut/(float)sample_rate));
				c->pos += c->speed;
				c->speed *=c->reso;
				//~filter

				out += c->data.vol*c->env*c->pos;
				out=out*32767;
				*buf++= (short)clamp(out, -32767, 32767);
				note = -1;
			}
		/*	{
				FILE *file = fopen("effect.wav", "wb");
				fwrite(wav, wave.size, 1, file);
				fclose(file);
			}
			*/
}


#else

#endif


