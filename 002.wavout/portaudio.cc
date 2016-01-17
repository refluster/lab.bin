#include <stdio.h>
#include <math.h>
#include <portaudio.h>
#include "wavReader.h"
#include "combFilter.h"
#include "allPassFilter.h"

#define NUM_SECONDS 3
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256

static WavReader *wr;
static CombFilter *cf;
static AllPassFilter *apf;

/* This routine will be called by the PortAudio engine when audio is needed.
   It may called at interrupt level on some machines so don't do anything
   that could mess up the system like calling malloc() or free().
*/ 
static int patestCallback( const void *inputBuffer, void *outputBuffer,
						   unsigned long framesPerBuffer,
						   const PaStreamCallbackTimeInfo* timeInfo,
						   PaStreamCallbackFlags statusFlags,
						   void *userData ) {
	/* Cast data passed through stream to our structure. */
	float *out = (float*)outputBuffer;
	float *in = (float*)inputBuffer;
	unsigned int i;
	(void) inputBuffer; /* Prevent unused variable warning. */

	static int count = 0;
	if (count++ == 44100 / 256) {
		count = 0;
		printf("-- sec --\n");
	}

	for(i = 0; i < framesPerBuffer; i++) {
		float _in;
		float _out;
		if (wr->read(&_in) != 0) {
			_in = 0.0f;
		}
		//cf->enqdeq(&_in, &_out);
		apf->enqdeq(&_in, &_out);
		*out++ = _out;
	}
	return 0;
}

int main() {
	PaStream *stream;
	PaError err;

	wr = new WavReader();
	cf = new CombFilter(22050, .8f);
	apf = new AllPassFilter(22050, .5f);

	err = Pa_Initialize();
	if (err != paNoError) {
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));
		return -1;
	}

	/* Open an audio I/O stream. */
	err = Pa_OpenDefaultStream(&stream,
							   0,          /* no input channels */
							   1,          /* mono output */
							   paFloat32,  /* 32 bit floating point output */
							   SAMPLE_RATE,
							   FRAMES_PER_BUFFER,        /* frames per buffer */
							   patestCallback, /* this is your callback function */
							   NULL );     /* This is a pointer that will be passed to your callback */
	if (err != paNoError) {
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));
		return -1;
	}

	err = Pa_StartStream(stream);
	if (err != paNoError) {
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));
		return -1;
	}

	/* Sleep for several seconds. */
	Pa_Sleep(NUM_SECONDS*1000);

	err = Pa_CloseStream(stream);
	if (err != paNoError) {
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));
		return -1;
	}

	err = Pa_Terminate();
	if (err != paNoError) {
		printf("PortAudio error: %s\n", Pa_GetErrorText(err));
	}

	delete wr;
	delete cf;
	delete apf;

	return 0;
}

