#ifndef _ALL_PASS_FILTER_H_
#define _ALL_PASS_FILTER_H_

class AllPassFilter {
	size_t size;
	float g;
	float *pRead, *pWrite;
	float *buf;

public:
	AllPassFilter(size_t size, float g);
	~AllPassFilter();	
	void enqdeq(float *in, float *out);
};

#endif
