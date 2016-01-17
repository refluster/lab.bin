#include "combFilter.h"
#include "allPassFilter.h"
#include "reverb.h"
#include "common.h"

// Schroeder Reverberators

Reverb::Reverb() {
	cf1 = new CombFilter((int)(CF1_DELAY_MSEC/1000*SAMPLE_RATE), CF1_GAIN);
	cf2 = new CombFilter((int)(CF2_DELAY_MSEC/1000*SAMPLE_RATE), CF2_GAIN);
	cf3 = new CombFilter((int)(CF3_DELAY_MSEC/1000*SAMPLE_RATE), CF3_GAIN);
	cf4 = new CombFilter((int)(CF4_DELAY_MSEC/1000*SAMPLE_RATE), CF4_GAIN);
	apf5 = new AllPassFilter((int)(APF5_DELAY_MSEC/1000*SAMPLE_RATE), APF5_GAIN);
	apf6 = new AllPassFilter((int)(APF6_DELAY_MSEC/1000*SAMPLE_RATE), APF6_GAIN);
};

Reverb::~Reverb() {
	delete cf1;
	delete cf2;
	delete cf3;
	delete cf4;
	delete apf5;
	delete apf6;
};

float Reverb::step(float in) {
	float cf1out, cf2out, cf3out, cf4out;
	float apf5in, apf5out, apf6out;
	
	cf1->enqdeq(&in, &cf1out);
	cf2->enqdeq(&in, &cf2out);
	cf3->enqdeq(&in, &cf3out);
	cf4->enqdeq(&in, &cf4out);

	apf5in = cf1out + cf2out + cf3out + cf4out;
	apf5->enqdeq(&apf5in, &apf5out);

	apf6->enqdeq(&apf5out, &apf6out);

	return in + apf6out;
};

#ifdef TEST
int main() {
	
}
#endif

