#include <stdio.h>
#include <math.h>
#include "portaudio.h"

#define NUM_SECONDS 3

typedef struct {
    float left_phase;
    float right_phase;
} paTestData;

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
    paTestData *data = (paTestData*)userData; 
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
#if 1
        *out++ = data->left_phase;  /* left */
        *out++ = data->right_phase;  /* right */
#else
        *out++ = *in++;
        *out++ = *in++;
#endif
        /* Generate simple sawtooth phaser that ranges between -1.0 and 1.0. */
        data->left_phase += 0.01f;
        data->right_phase += 0.03f;
#if 0
        /* When signal reaches top, drop back down. */
        if( data->left_phase >= 1.0f ) data->left_phase -= 2.0f;
        /* higher pitch so we can distinguish left and right. */
        if( data->right_phase >= 1.0f ) data->right_phase -= 2.0f;
#else
        /* When signal reaches top, drop back down. */
        if( data->left_phase >= .1f ) data->left_phase -= .2f;
        /* higher pitch so we can distinguish left and right. */
        if( data->right_phase >= .1f ) data->right_phase -= .2f;
#endif
    }
    return 0;
}

#define SAMPLE_RATE (44100)
static paTestData data;

int f() {
    PaStream *stream;
    PaError err;
    /* Open an audio I/O stream. */
    err = Pa_OpenDefaultStream( &stream,
#if 1
                                0,          /* no input channels */
#else
                                2,          /* no input channels */
#endif
                                2,          /* stereo output */
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
                                &data ); /*This is a pointer that will be passed to
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

	return 0;
}
