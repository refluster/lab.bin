#include <stdio.h>
#include <stdlib.h>
#include "combFilter.h"
#include "common.h"

CombFilter::CombFilter(size_t size, float g) {
	this->size = size;
	this->g = g;
	this->buf = (float*)calloc(size, sizeof(float));
	this->pRead = this->pWrite = this->buf;
};

CombFilter::~CombFilter() {
	free(this->buf);
};

void CombFilter::enqdeq(float *in, float *out) {
	*out = *this->pRead;

	*this->pWrite = *in + this->g * (*this->pRead);

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
	CombFilter *cf = new CombFilter(4, .5f);
	float in[30] = {1, 2, 3, 4, 5, 6, 0, 0, 0, 0};
	float out;
	
	for (int i = 0; i < 30; i++) {
		cf->enqdeq(&in[i], &out);

		printf("-- %2d %3f\n", i, out);
	}
}
#endif
