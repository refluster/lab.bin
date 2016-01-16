#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>

int main(int argc, char *argv[]) {
	printf("Wav Read Test\n");
	if (argc != 2) {
		fprintf(stderr, "Expecting wav file as argument\n");
		return 1;
	}

	// Open sound file
	SF_INFO sndInfo;
	SNDFILE *sndFile = sf_open(argv[1], SFM_READ, &sndInfo);
	if (sndFile == NULL) {
		fprintf(stderr, "Error reading source file '%s': %s\n", argv[1], sf_strerror(sndFile));
		return 1;
	}

	// Check format - 16bit PCM
	if (sndInfo.format != (SF_FORMAT_WAV | SF_FORMAT_PCM_16)) {
		fprintf(stderr, "Input should be 16bit Wav\n");
		sf_close(sndFile);
		return 1;
	}
	
	// Check channels - mono
	if (sndInfo.channels != 1) {
		fprintf(stderr, "Wrong number of channels\n");
		sf_close(sndFile);
		return 1;
	}

	// Allocate memory
	float *buffer = malloc(sndInfo.frames * sizeof(float));
	if (buffer == NULL) {
		fprintf(stderr, "Could not allocate memory for data\n");
		sf_close(sndFile);
		return 1;
	}

	// Load data
	long numFrames = sf_readf_float(sndFile, buffer, sndInfo.frames);
	
	// Check correct number of samples loaded
	if (numFrames != sndInfo.frames) {
		fprintf(stderr, "Did not read enough frames for source\n");
		sf_close(sndFile);
		free(buffer);
		return 1;
	}

	// Output Info
	printf("Read %ld frames from %s, Sample rate: %d, Length: %fs\n",
		   numFrames, argv[1], sndInfo.samplerate, (float)numFrames/sndInfo.samplerate);
	
	sf_close(sndFile);
	free(buffer);
	
	return 0;
}
