#include "minifmod170/lib/minifmod.h"
#include "sound.h"
//#include <stdio.h>
#include <windows.h>
#include "synth.h"
#include <mmsystem.h> 
#include "dx/DSOUND.h"

#include "songs/wonder_recycled.h"
#include "songs/wonder_raptors.h"
#define USEMEMLOAD


int g_MusicIndex = 0;

#define USEMEMLOADRESOURCE
#if 0 
unsigned int fileopen(char *name)
{
	return (unsigned int)fopen(name, "rb");
}

void fileclose(unsigned int handle)
{
	fclose((FILE *)handle);
}

int fileread(void *buffer, int size, unsigned int handle)
{
	return fread(buffer, 1, size, (FILE *)handle);
}

void fileseek(unsigned int handle, int pos, signed char mode)
{
	fseek((FILE *)handle, pos, mode);
}

int filetell(unsigned int handle)
{
	return ftell((FILE *)handle);
}

#else



typedef struct 
{
	int length;
	int pos;
	void *data;
} MEMFILE;


unsigned int memopen(char *name)
{
	MEMFILE *memfile;

	memfile = (MEMFILE *)calloc(sizeof(MEMFILE),1);

#ifndef USEMEMLOADRESOURCE
	{	// load an external file and read it
		FILE *fp;
		fp = fopen(name, "rb");
		if (fp)
		{
			fseek(fp, 0, SEEK_END);
			memfile->length = ftell(fp);
			memfile->data = calloc(memfile->length,1);
			memfile->pos = 0;

			fseek(fp, 0, SEEK_SET);
			fread(memfile->data, 1, memfile->length, fp);
			fclose(fp);
		}
	}
#else
	{	// hey look some load from resource code!
//		HRSRC		rec;
//		HGLOBAL		handle;
		unsigned char* mapper[] = { wonder_recycled,wonder_raptors};
	//	rec = FindResource(NULL, MAKEINTRESOURCE(IDR_XMFILE2), "XMFILE");
	//	rec = FindResource(NULL, name, "XMFILE");
//		rec = FindResource(NULL, MAKEINTRESOURCE(mapper[g_MusicIndex]), "XMFILE");
//		handle = LoadResource(NULL, rec);
		
		memfile->pos = 0;
		if(g_MusicIndex  == 0)
		{
			memfile->data = wonder_recycled;
			memfile->length = sizeof(wonder_recycled);
		
		} 
		else
		{
			memfile->data = wonder_raptors;
			memfile->length = sizeof(wonder_raptors);
		}
	}
#endif

	return (unsigned int)memfile;
}

void memclose(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

#ifndef USEMEMLOADRESOURCE
	free(memfile->data);			// dont free it if it was initialized with LockResource
#endif

	free(memfile);
}

int memread(void *buffer, int size, unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (memfile->pos + size >= memfile->length)
		size = memfile->length - memfile->pos;

	memcpy(buffer, (char *)memfile->data+memfile->pos, size);
	memfile->pos += size;
	
	return size;
}

void memseek(unsigned int handle, int pos, signed char mode)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	if (mode == SEEK_SET) 
		memfile->pos = pos;
	else if (mode == SEEK_CUR) 
		memfile->pos += pos;
	else if (mode == SEEK_END)
		memfile->pos = memfile->length + pos;

	if (memfile->pos > memfile->length)
		memfile->pos = memfile->length;
}

int memtell(unsigned int handle)
{
	MEMFILE *memfile = (MEMFILE *)handle;

	return memfile->pos;
}
#endif


//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------


FMUSIC_MODULE *mod;


//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

void SOUND_play_song(int idx)
{
	g_MusicIndex = idx ;
	mod = FMUSIC_LoadSong("", NULL); 
	FMUSIC_PlaySong(mod);

}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

void SOUND_stop_song()
{
	FMUSIC_StopSong(mod);
	FMUSIC_FreeSong(mod);
}

LPDIRECTSOUND lpDirectSound = NULL;
WAVEFORMATEX pcmwf;
DSBUFFERDESC dsbdesc;

const int SR = 44100;
const int bits = 16;


HRESULT (WINAPI *pDirectSoundCreate)(GUID FAR *lpGUID, LPDIRECTSOUND FAR *lplpDS, IUnknown FAR *pUnkOuter);

//from quake source
void LoadDsound()
{
	HINSTANCE hInstDS;
	hInstDS = LoadLibrary("dsound.dll");

    if (hInstDS == 0)
    {
     
    }

    pDirectSoundCreate = (void *)GetProcAddress(hInstDS,"DirectSoundCreate");

    if (!pDirectSoundCreate)
    {
     
    }
}


//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

int DirectSoundInit()
{
   HRESULT hr;
   HWND hWnd;

	LoadDsound();	
   hr = pDirectSoundCreate(NULL, &lpDirectSound, NULL);
   if( hr != DS_OK ) 
	   return 0;
   hWnd = GetForegroundWindow();
   // establecer el nivel de cooperacion
   hr = IDirectSound_SetCooperativeLevel(lpDirectSound,hWnd, DSSCL_NORMAL );
   if( hr != DS_OK)
		return 0;
   return 1;

 
}
int FillDSBufer(LPDIRECTSOUNDBUFFER pDSB,void* buffer, int ndata);
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
LPDIRECTSOUNDBUFFER CreateStaticSoundBuffer(void* buffer, int ndata)
{

  LPDIRECTSOUNDBUFFER pDSB = NULL;;
  memset( &pcmwf, 0, sizeof (WAVEFORMATEX ));
  pcmwf.wFormatTag = WAVE_FORMAT_PCM;
  pcmwf.nChannels = 1;
  pcmwf.nSamplesPerSec = SR;
  pcmwf.nBlockAlign = 2;
  pcmwf.nAvgBytesPerSec = SR * 2;
  pcmwf.wBitsPerSample = bits;

	// --- start sound ---

	// Set up DSBUFFERDESC structure.
  memset(&dsbdesc, 0, sizeof (DSBUFFERDESC)); // Zero it out.
  dsbdesc.dwSize = sizeof (DSBUFFERDESC);
  dsbdesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLFREQUENCY;// |  DSBCAPS_GETCURRENTPOSITION2;
/*
  #define DSBCAPS_CTRLFREQUENCY       0x00000020
#define DSBCAPS_CTRLPAN             0x00000040
#define DSBCAPS_CTRLVOLUME          0x00000080
  */
  dsbdesc.dwBufferBytes = ndata;
  dsbdesc.lpwfxFormat = &pcmwf;
  
  if(!SUCCEEDED(IDirectSound_CreateSoundBuffer(lpDirectSound,&dsbdesc, &(pDSB), NULL)))
  {
	  MessageBox(0,"ERROR BUFFER","ERROR BUFFER",MB_OK);
	  return NULL;

  }
  FillDSBufer(pDSB,buffer,ndata);
  return pDSB;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

int FillDSBufer(LPDIRECTSOUNDBUFFER pDSB,void* buffer, int ndata)
{
		LPVOID pMem1, pMem2;
        DWORD dwSize1, dwSize2;

        if (SUCCEEDED(IDirectSoundBuffer_Lock(pDSB, 0, ndata,
            &pMem1, &dwSize1, &pMem2, &dwSize2, 0)))
        {
            CopyMemory(pMem1, buffer, dwSize1);

            if ( 0 != dwSize2 )
                CopyMemory(pMem2, ((unsigned char*)buffer)+dwSize1, dwSize2);

            IDirectSoundBuffer_Unlock(pDSB, pMem1, dwSize1, pMem2, dwSize2);
			return 1;
		}
		return 0;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

LPDIRECTSOUNDBUFFER CreateSoundEffect(struct Channel *c, int samples,int note)
{
  LPDIRECTSOUNDBUFFER dsB;
  short* mem = malloc(sizeof(short)*samples);
  synth_effect(mem,c,note,samples); 
  dsB = CreateStaticSoundBuffer(mem,samples*2);
  return dsB;
  
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

int SOUND_init()
{

	

#ifdef USEFMOD

#ifndef USEMEMLOAD
	FSOUND_File_SetCallbacks(fileopen, fileclose, fileread, fileseek, filetell);
#else
	FSOUND_File_SetCallbacks(memopen, memclose, memread, memseek, memtell);
#endif
#endif


	if(!DirectSoundInit())
	{
		MessageBox(0,"error al iniciar DS","ERROR",MB_OK);
	}
	return 1;

}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------

sample sample_generator[MAX_SAMPLE_GENERATOR];
int nsample = 0;

LPDIRECTSOUNDBUFFER sample_buffers[MAX_BUFFERS];
int nbuffers = 0;

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void SOUND_add_sample(struct Channel *c, int samples,int note)
{
	sample_generator[nsample].c = *c;
	sample_generator[nsample].samples = samples;
	sample_generator[nsample].note = note;
	nsample++;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
int SOUND_create_sample(int sample_id)
{
	sample* s = &sample_generator[sample_id];
	sample_buffers[nbuffers] =  CreateSoundEffect(&s->c, s->samples,s->note);
	return nbuffers++;

}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void SOUND_play(int sound,int mode)
{
	int m;
	if(mode == PLAY_LOOP)
		m = DSBPLAY_LOOPING;
	else 
		m = 0;
	IDirectSoundBuffer_Play(sample_buffers[sound],0,0,m);
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
void SOUND_stop(int sound)
{
	IDirectSoundBuffer_Stop(sample_buffers[sound]);

}

void SOUND_set_frequency(int sound, int freq)
{
	IDirectSoundBuffer_SetFrequency(sample_buffers[sound],freq) ;
}

void SOUND_set_vol(int sound, float vol)
{
	IDirectSoundBuffer_SetVolume(sample_buffers[sound], (LONG)vol) ;
}




