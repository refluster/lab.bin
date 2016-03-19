#ifndef _COMB_FILTER_H_
#define _COMB_FILTER_H_

#include <stdio.h>

class CombFilter {
	size_t size;
	float g;
	float *pRead, *pWrite;
	float *buf;

public:
	CombFilter(size_t size, float g);
	~CombFilter();	
	void enqdeq(float *in, float *out);
};

#endif
