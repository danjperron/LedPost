#include "LedInfo.h"

// force to use bank 0 for interrupt speed increase  (no need to use movlb)
#pragma udata  ledsbank
unsigned char LedPWMCounter; // counter use by interrupt for PWM output
LedsChannelUnion LedsChannel;// 3 channels per led and 4 leds
#pragma udata
unsigned short RGB555Led[MAX_LED];