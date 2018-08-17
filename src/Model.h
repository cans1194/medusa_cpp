#include "mbed.h"
#include "Adafruit_SSD1306.h"


class Model 
{
public: 
    Model(Adafruit_SSD1306_I2c *display_driver);

		typedef struct 
		{
			uint8_t frame_size;
			const uint16_t *data;
		}model_t;

		model_t heal_model;
		model_t illness_model;
		model_t death_model;
		model_t hide_model;
		
		void drawHeal(uint8_t x, uint8_t y, uint8_t frame, uint16_t color);

private:
    Adafruit_SSD1306_I2c *driver;
		
		void drawModel(uint8_t x, uint8_t y, model_t *model, uint8_t frame, uint16_t color);

		/*Models data*/
		static uint16_t heal[];
		static uint16_t illness[];
		static uint16_t death[];
		static uint16_t hide[];
};