#include <stdio.h>
#include <math.h>
#include "portaudio.h"

#define NUM_SECONDS 3

int wavInit();
int wavRead(float*);
void wavFini();

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
    
	//printf("fpB: %lu\n", framesPerBuffer);
	static int count = 0;

	if (count++ == 44100 / 256) {
		count = 0;
		printf("-- sec --\n");
	}

    for( i=0; i<framesPerBuffer; i++ ) {
#if 0
        *out++ = data->left_phase;  /* left */
        *out++ = data->right_phase;  /* right */
#else
//		*out = wavRead();
//		*out++ = 0;
		{
			float f;
			if (wavRead(&f) != 0) {
				f = 0;
			}
//			printf("%f\n", f);
			*out++ = f;

		}
#endif
    }
    return 0;
}

#define SAMPLE_RATE (44100)

int f() {
    PaStream *stream;
    PaError err;
    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
                                0,          /* no input channels */
#if 0
                                2,          /* stereo output */
#else
                                1,          /* mono output */
#endif								
                                paFloat32,  /* 32 bit floating point output */
                                SAMPLE_RATE,
                                256,        /* frames per buffer, i.e. the number
                                                   of sample frames that PortAudio will
                                                   request from the callback. Many apps
                                                   may want to use
                                                   paFramesPerBufferUnspecified, which
                                                   tells PortAudio to pick the best,
                                                   possibly changing, buffer size.*/
                                patestCallback, /* this is your callback function */
                                NULL ); /*This is a pointer that will be passed to
                                                   your callback*/
	if( err != paNoError ) {
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return -1;
	}

	err = Pa_StartStream( stream );
	if( err != paNoError ) {
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return -1;
	}

	/* Sleep for several seconds. */
	Pa_Sleep(NUM_SECONDS*1000);

	err = Pa_CloseStream( stream );
	if( err != paNoError ) {
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return -1;
	}

	return 0;
}

int main() {
    PaError err;

	wavInit();

	err = Pa_Initialize();
	if( err != paNoError ) {
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return -1;
	}
	
	f();

	err = Pa_Terminate();
	if( err != paNoError ) {
		printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
	}

	wavFini();

	return 0;
}
