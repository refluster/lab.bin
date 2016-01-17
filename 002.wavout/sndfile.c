#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>

static struct {
	float *ptr;
	float *bufStart;
	float *bufEnd;
	SNDFILE *sndFile;
	SF_INFO sndInfo;
} stat;

int wavInit() {
	// Open sound file
	const char *fname = "water-drop1.wav";

	stat.sndFile = sf_open(fname, SFM_READ, &stat.sndInfo);

	if (stat.sndFile == NULL) {
		fprintf(stderr, "Error reading source file '%s': %s\n", fname, sf_strerror(stat.sndFile));
		return 1;
	}

	// Check format - 16bit PCM
	if (stat.sndInfo.format != (SF_FORMAT_WAV | SF_FORMAT_PCM_16)) {
		fprintf(stderr, "Input should be 16bit Wav\n");
		sf_close(stat.sndFile);
		return 1;
	}

	// Check channels - mono
	if (stat.sndInfo.channels != 1) {
		fprintf(stderr, "Wrong number of channels\n");
		sf_close(stat.sndFile);
		return 1;
	}

	// Allocate memory
	stat.bufStart = malloc(stat.sndInfo.frames * sizeof(float));
	stat.bufEnd = stat.bufStart + stat.sndInfo.frames;
	stat.ptr = stat.bufEnd;
	if (stat.bufStart == NULL) {
		fprintf(stderr, "Could not allocate memory for data\n");
		sf_close(stat.sndFile);
		return 1;
	}

	return 0;
}

int wavRead(float *v) {
	*v = 0;

	if (stat.ptr == stat.bufEnd) {
		// Load data
		long numFrames = sf_readf_float(stat.sndFile, stat.bufStart, stat.sndInfo.frames);

		// Check correct number of samples loaded
		if (numFrames != stat.sndInfo.frames) {
			//fprintf(stderr, "Did not read enough frames for source\n");
			//sf_close(stat.sndFile);
			//free(stat.bufStart);
			return 1;
		}

		stat.ptr = stat.bufStart;
	} else {
		++stat.ptr;
	}

	*v = *stat.ptr;
	return 0;
}

void wavFini() {
	sf_close(stat.sndFile);
	free(stat.bufStart);
}
