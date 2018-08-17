

#ifndef __ChainableLED_h__
#define __ChainableLED_h__

#include "mbed.h"


typedef    char  byte;
#define _CL_RED             0
#define _CL_GREEN           1
#define _CL_BLUE            2
#define _CLK_PULSE_DELAY    20

#define PWR_ENABLE          0
#define PWR_DISABLE         1

void ChainableLED(byte number_of_leds);    
void setColorRGB(byte led, byte red, byte green, byte blue);
void setColorHSB(byte led, float hue, float saturation, float brightness);
void pwr_set(byte mode);

void clk(void);
void sendByte(byte b);
void sendColor(byte red, byte green, byte blue);

#endif

