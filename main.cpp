/*********************************************************************************************************
*
* File                : main.cpp
* Hardware Environment: 
* Build Environment   : Keil MDK
* Version             : V5.15
* By                  : WaveShare
*
*                                  (c) Copyright 2005-2014, WaveShare
*                                       http://www.waveshare.net
*                                       http://www.waveshare.com
*                                          All Rights Reserved
*
*********************************************************************************************************/
#include <stdlib.h>

#include "mbed.h"
#include "Adafruit_SSD1306.h"
#include "ChainableLED.h"
#include "lm75.h"
#include "ADXL345_I2C.h"
#include "math.h"

#include "Model.h"
 
DigitalOut myled(LED1);
PwmOut beep_pin(D11);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define    KEY_UP        A1
#define    KEY_DOWN      A5
#define    KEY_LEFT      A3
#define    KEY_RIGHT     A2
#define    KEY_ENTER     A4

// an I2C sub-class that provides a constructed default
class I2CPreInit : public I2C
{
public:
    I2CPreInit(PinName sda, PinName scl) : I2C(sda, scl)
    {
        frequency(400000);
    };
};
I2CPreInit gI2C(I2C_SDA, I2C_SCL);
Adafruit_SSD1306_I2c display(gI2C, D7, D8);
Serial uart(USBTX, USBRX);
TempI2C_LM75 termo = TempI2C_LM75(0x48,TempI2C_LM75::nine_bits);
DigitalIn up(KEY_UP), down(KEY_DOWN), left(KEY_LEFT), right(KEY_RIGHT), enter(KEY_ENTER);
ADXL345_I2C acc(I2C_SDA, I2C_SCL);

Model model(&display);

struct time
{
	int mili;
	int second;
	int minutes;
	int hour;
};
/**/
typedef struct
{
	uint8_t x;
	uint8_t y;
	uint8_t delta;
}object;
/**/
struct game
{
	int time_enable;
	int health;
	int health_sc;
	int illness;
	int illness_sc;
};
/**/
void counter_time()
{
	static struct time _time = {.second=0, .minutes=0, .hour=0};
	
	_time.mili++;
	if (100 == _time.mili)
	{
		_time.second++;
		_time.mili =0;
	}
	if (60 == _time.second)
	{
		_time.second = 0;
		_time.minutes++;
	}
	if (60 == _time.minutes)
	{
		_time.minutes = 0;
		_time.hour++;
	}
	display.setTextSize(1);
  display.setTextCursor(0, 5);
	display.printf("%.2d:%.2d:%.2d", 
									_time.hour,
									_time.minutes,
									_time.second);
}

/**/
void counter(struct game *data)
{
	display.setTextSize(1);
	display.setTextCursor(80, 0);
	display.printf("%.3dh%.3di",
									data->health_sc,
									data->illness_sc);
	display.setTextCursor(80, 8);
	display.printf("%.3dh%.3di",
									data->health,
									data->illness);
}
/**/
void control_coord(int coord[])
{
	if (coord[0] == display.width())
		coord[0] = display.width();
	if ((int*)coord[0] <= 0)
		coord[0] = 0;
	if (coord[1] == display.height())
		coord[1] = display.height();
	if ((int*)coord[1] <= 0)
		coord[1] = 0;
}
/**/
void draw_triangle(int coord[], float phase, int radius)
{
	int x[2], y[2] ,z[2];
	x[0] = sin(phase) * radius + coord[0];
	x[1] = cos(phase) * radius + coord[1];
	y[0] = sin(phase + (6.18/3)) * radius + coord[0];
	y[1] = cos(phase + (6.18/3)) * radius + coord[1];
	z[0] = sin(phase - (6.18/3)) * radius + coord[0];
	z[1] = cos(phase - (6.18/3)) * radius + coord[1];
	
	display.drawTriangle( x[0], x[1],
												y[0], y[1],
												z[0], z[1], WHITE);
}
/**/
void drop_balls()
{   
		/*static struct ball _ball = {.x=0, .y=0, .next =NULL};
		int i=0;
		_ball.x++;
		while (_ball->next != NULL)
		{
			ball->next
		}*/
}
/**/









/*MAIN CYCLE*/
int main()
{
    int _temp = 0;
    int coord[3] = {display.width()/2, display.height()/2};
		
	struct game GAMEpar = {0,0,0,0,0};
		
		
    uart.baud(9600);
    up.mode(PullUp);
    down.mode(PullUp);
    left.mode(PullUp);
    right.mode(PullUp);
    enter.mode(PullUp);
    ChainableLED(1);
    pwr_set(PWR_ENABLE);
	/*ACC*/
	acc.setPowerControl(0x00);
	wait_ms(10);
	acc.setDataFormatControl(0x0B);
	wait_ms(10);
	acc.setPowerControl(MeasurementMode);
    /*TEST*/
    /*for (int i = 0; i < 5; i++)
    {
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextCursor(40, 20);
        display.printf("TEST");
        display.setTextSize(1);
        display.setTextCursor(51, 40);
        display.printf("00:0%d", i);
        display.display();
        setColorRGB(0, rand()%255, rand()%255, rand()%255);
        wait(1);
        display.clearDisplay();
		}*/
    /*TEST COMPLETE*/
    beep_pin.period_ms(20);
    beep_pin.pulsewidth(20/2);
    display.setTextSize(2);
    display.setTextCursor(40, 20);
    display.printf("TEST");
    display.setTextCursor(20, 40);
    display.printf("COMPLETE");
    display.display();
    wait_ms(500);
    beep_pin.pulsewidth(0);
    display.clearDisplay();
    setColorRGB(0, 0,0,0);
    
    /*////////////////////*/
	float phase = 0;
	int radius 	= 12;
	/**/
    while(1)
    {
		display.clearDisplay();
		if (left == 0)
			coord[0]--;
		if (right ==0)
			coord[0]++;
		if (up == 0)
			coord[1]--;
		if (down == 0)
			coord[1]++;
		if (enter == 0)
			(_temp==0)?_temp = 1:_temp = 0;
				
		if (!_temp)
		{
			counter_time();
			counter(&GAMEpar);
		}
		control_coord(coord);
				
		/*balls*/
		phase += 3.14 / 36;
		if (phase > 6.18)
			phase = 0;
		draw_triangle(coord, phase, radius);
		model.drawHeal(30, 40, 4, WHITE);
		display.display();
	}
}

