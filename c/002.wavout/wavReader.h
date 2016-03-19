#ifndef _SNDFILE_H_
#define _SNDFILE_H_

#include <sndfile.h>

class WavReader {
private:
	float *ptr;
	float *bufStart;
	float *bufEnd;
	SNDFILE *sndFile;
	SF_INFO sndInfo;

public:
	WavReader(char *fname);
	~WavReader();
	int read(float *v);
};

#endif
