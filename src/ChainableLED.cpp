#include "ChainableLED.h"


DigitalOut io_clk(D6), io_data(D5), io_pwr(D12);
byte _num_leds; 
byte _led_state[20];



// Forward declaration
float hue2rgb(float p, float q, float t);
float constrain(float x, float a, float b);

// --------------------------------------------------------------------------------------


void ChainableLED(byte number_of_leds)
{
	_num_leds = number_of_leds;
	io_pwr = 0;
	for (byte i=0; i<_num_leds; i++)
		setColorRGB(i, 0, 0, 0);
}


// --------------------------------------------------------------------------------------

void clk(void)
{
    io_clk = 0;
    wait_us(_CLK_PULSE_DELAY); 
	  io_clk = 1;
    wait_us(_CLK_PULSE_DELAY);   
}

void sendByte(byte b)
{
    // Send one bit at a time, starting with the MSB
    for (byte i=0; i<8; i++)
    {
        // If MSB is 1, write one and clock it, else write 0 and clock
        if ((b & 0x80) != 0)
				    io_data = 1;
        else
				    io_data = 0;
        clk();

        // Advance to the next bit to send
        b <<= 1;
    }
}
 
void sendColor(byte red, byte green, byte blue)
{
    // Start by sending a byte with the format "1 1 /B7 /B6 /G7 /G6 /R7 /R6"
    byte prefix = 0xC0;
    if ((blue & 0x80) == 0)     prefix|= 0x20;
    if ((blue & 0x40) == 0)     prefix|= 0x10; 
    if ((green & 0x80) == 0)    prefix|= 0x08;
    if ((green & 0x40) == 0)    prefix|= 0x04;
    if ((red & 0x80) == 0)      prefix|= 0x02;
    if ((red & 0x40) == 0)      prefix|= 0x01;
    sendByte(prefix);
        
    // Now must send the 3 colors
    sendByte(blue);
    sendByte(green);
    sendByte(red);
}

void setColorRGB(byte led, byte red, byte green, byte blue)
{
    // Send data frame prefix (32x "0")
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    
    // Send color data for each one of the leds
    for (byte i=0; i<_num_leds; i++)
    {
        if (i == led)
        {
            _led_state[i*3 + _CL_RED] = red;
            _led_state[i*3 + _CL_GREEN] = green;
            _led_state[i*3 + _CL_BLUE] = blue;
        }
                    
        sendColor(_led_state[i*3 + _CL_RED], 
                  _led_state[i*3 + _CL_GREEN], 
                  _led_state[i*3 + _CL_BLUE]);
    }

    // Terminate data frame (32x "0")
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
    sendByte(0x00);
}

void setColorHSB(byte led, float hue, float saturation, float brightness)
{
    float r, g, b;
    
    hue = constrain(hue, 0.0f, 1.0f);
    saturation = constrain(saturation, 0.0f, 1.0f);
    brightness = constrain(brightness, 0.0f, 1.0f);

    if(saturation == 0.0f)
    {
        r = g = b = brightness;
    }
    else
    {
        float q = brightness < 0.5f ? 
            brightness * (1.0f + saturation) : brightness + saturation - brightness * saturation;
        float p = 2.0f * brightness - q;
        r = hue2rgb(p, q, hue + 1.0f/3.0f);
        g = hue2rgb(p, q, hue);
        b = hue2rgb(p, q, hue - 1.0f/3.0f);
    }

    setColorRGB(led, (byte)(255.0f*r), (byte)(255.0f*g), (byte)(255.0f*b));
}
void pwr_set(byte mode)
{
  if(mode == PWR_ENABLE)
	  io_pwr = 1;
	else
		io_pwr = 0;
}


// --------------------------------------------------------------------------------------

float hue2rgb(float p, float q, float t)
{
    if (t < 0.0f) 
        t += 1.0f;
    if(t > 1.0f) 
        t -= 1.0f;
    if(t < 1.0f/6.0f) 
        return p + (q - p) * 6.0f * t;
    if(t < 1.0f/2.0f) 
        return q;
    if(t < 2.0f/3.0f) 
        return p + (q - p) * (2.0f/3.0f - t) * 6.0f;

    return p;
}
float constrain(float x, float a, float b)
{
	if(x < a) return a;
  else if(x > b) return b;
  else return x;	
}


