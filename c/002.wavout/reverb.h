#ifndef _REVERB_H_
#define _REVERB_H_

#include "combFilter.h"
#include "allPassFilter.h"

#define CF1_DELAY_MSEC 39.85f
#define CF2_DELAY_MSEC 36.10f
#define CF3_DELAY_MSEC 33.27f
#define CF4_DELAY_MSEC 30.15f
#define APF5_DELAY_MSEC 5.0f
#define APF6_DELAY_MSEC 1.7f

#define CF1_GAIN 0.871402f
#define CF2_GAIN 0.882762f
#define CF3_GAIN 0.891443f
#define CF4_GAIN 0.901117f
#define APF5_GAIN 0.9f
#define APF6_GAIN 0.7f

// Schroeder Reverberators

class Reverb {
private:
	CombFilter *cf1;
	CombFilter *cf2;
	CombFilter *cf3;
	CombFilter *cf4;
	AllPassFilter *apf5;
	AllPassFilter *apf6;

public:
	Reverb();
	~Reverb();
	float step(float in);
};

#endif
