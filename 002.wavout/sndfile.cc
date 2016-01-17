#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include "sndfile.h"

int WavReader::init() {
	// Open sound file
	const char *fname = "water-drop1.wav";

	this->sndFile = sf_open(fname, SFM_READ, &this->sndInfo);

	if (this->sndFile == NULL) {
		fprintf(stderr, "Error reading source file '%s': %s\n", fname, sf_strerror(this->sndFile));
		return 1;
	}

	// Check format - 16bit PCM
	if (this->sndInfo.format != (SF_FORMAT_WAV | SF_FORMAT_PCM_16)) {
		fprintf(stderr, "Input should be 16bit Wav\n");
		sf_close(this->sndFile);
		return 1;
	}

	// Check channels - mono
	if (this->sndInfo.channels != 1) {
		fprintf(stderr, "Wrong number of channels\n");
		sf_close(this->sndFile);
		return 1;
	}

	// Allocate memory
	this->bufStart = (float*)malloc(this->sndInfo.frames * sizeof(float));
	this->bufEnd = this->bufStart + this->sndInfo.frames;
	this->ptr = this->bufEnd;
	if (this->bufStart == NULL) {
		fprintf(stderr, "Could not allocate memory for data\n");
		sf_close(this->sndFile);
		return 1;
	}

	return 0;
}

int WavReader::read(float *v) {
	*v = 0;

	if (this->ptr == this->bufEnd) {
		// Load data
		long numFrames = sf_readf_float(this->sndFile, this->bufStart, this->sndInfo.frames);

		// Check correct number of samples loaded
		if (numFrames != this->sndInfo.frames) {
			//fprintf(stderr, "Did not read enough frames for source\n");
			//sf_close(this->sndFile);
			//free(this->bufStart);
			return 1;
		}

		this->ptr = this->bufStart;
	} else {
		++this->ptr;
	}

	*v = *this->ptr;
	return 0;
}

void WavReader::fini() {
	sf_close(this->sndFile);
	free(this->bufStart);
}
