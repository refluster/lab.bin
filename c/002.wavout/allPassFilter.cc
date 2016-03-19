#include <stdio.h>
#include <stdlib.h>
#include "allPassFilter.h"
#include "common.h"

AllPassFilter::AllPassFilter(size_t size, float g) {
	this->size = size;
	this->g = g;
	this->buf = (float*)calloc(size, sizeof(float));
	this->pRead = this->pWrite = this->buf;
};

AllPassFilter::~AllPassFilter() {
	free(this->buf);
};

void AllPassFilter::enqdeq(float *in, float *out) {
	const float tmp = *in + this->g * (*this->pRead);

	*out = (1 - this->g) * tmp + *this->pRead;

	*this->pWrite = tmp;

	++this->pWrite;
	++this->pRead;
	if (this->pWrite >= this->buf + this->size) {
		this->pWrite = this->buf;
	}
	if (this->pRead >= this->buf + this->size) {
		this->pRead = this->buf;
	}
};

#ifdef TEST
int main() {
	AllPassFilter *cf = new AllPassFilter(4, 1);
	float in[30] = {1, 2, 3, 4, 5, 6, 0, 0, 0, 0};
	float out;
	
	for (int i = 0; i < 30; i++) {
		cf->enqdeq(&in[i], &out);

		printf("-- %2d %3f\n", i, out);
	}
}
#endif
