//TODO: HIGH PRIORITY; spin all SSD1306 functions into their own file, and integrate via header files
//INFO: all SSD1306 functions rely on a 128x8, uint8 array. in this array, each individual *bit* is a single pixel
//TODO: ISR call of Update_Whole_Display X times per second
//TODO: write functions to draw squares, lines, circles etc.
//TODO: LOW PRIORITY; less naive integration of drawing functions to speed up drawing
//(currently speed is adequate, must do testing on speediness before rewrite drawing.

#include "main.h"

I2C_HandleTypeDef * hi2c;

void I2C_SSD1306_Screen_Init(I2C_HandleTypeDef * hi2c);
void I2C_SSD1306_Update_Whole_Display(uint8_t SSD1306_FrameBufferPages[128][8]);
void I2C_SSD1306_Screen_Transmit(uint16_t data_len, uint8_t *data);

/**
 * @brief I2C initilisation of a small OLED screen
 * @param None
 * @retval None
 */
void I2C_SSD1306_Screen_Init(I2C_HandleTypeDef * hi2c1) {
	hi2c = hi2c1;
	//Reference manual for initialisation sequence can be found here, pg 64 (https://www.alldatasheet.com/datasheet-pdf/view/1179026/ETC2/SSD1306.html)

	uint8_t data[2];

	// Set the MUX ratio (0xA8) to 0x3F
	data[0] = 0x80;
	data[1] = 0xA8;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x3F;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set the Display Offset (0xD3) to 0x00
	data[0] = 0x80;
	data[1] = 0xD3;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x00;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Display Start Line (0x40)
	data[0] = 0x80;
	data[1] = 0x40;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Segment re-map (0xA1)
	data[0] = 0x80;
	data[1] = 0xA1;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// I2C_SSD1306_Screen_Transmit(ection (0xC8)
	data[0] = 0x80;
	data[1] = 0xC8;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set COM hardware configuration (0xDA) to 0x12
	data[0] = 0x80;
	data[1] = 0xDA;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x12;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Contrast Control (0x81) to 0x7F
	data[0] = 0x80;
	data[1] = 0x81;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x7F;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Disable Entire Display On (0xA4)
	data[0] = 0x80;
	data[1] = 0xA4;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Normal Display (0xA6)
	data[0] = 0x80;
	data[1] = 0xA6;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set OSC Frequency (0xD5) to 0x80
	data[0] = 0x80;
	data[1] = 0xD5;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x80;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Enable Charge Pump Regulator (0xAD) to 0x14
	data[0] = 0x80;
	data[1] = 0x8D;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x14;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Display On (0xAF)
	data[0] = 0x80;
	data[1] = 0xAF;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

	// Set Direction for pages (0x20) to columns (0x01)
	data[0] = 0x80;
	data[1] = 0x20;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);
	data[0] = 0x80;
	data[1] = 0x01;
	I2C_SSD1306_Screen_Transmit((uint16_t) 2, data);

}

/**
 * @brief transmission of data over I2C to OLED screen
 * @param data_len number of frames to transmit, data actual data to transmit
 * @retval None
 */

void I2C_SSD1306_Update_Whole_Display(uint8_t SSD1306_FrameBufferPages[128][8]) {
	// Set the columns and pages to be written to

	// 0x80 - Control byte
	// 0x21 - Set Column
	// 0x80 - Control byte
	// 0x0 - Set starting column to 0
	// 0x80 - Control byte
	// 127 - Set end column to 127
	// ---------------------------
	// 0x80 - Control byte
	// 0x22 - Set page
	// 0x80 - Control byte
	// 0x0 - Set starting page to 0
	// 0x80 - Control byte
	// 0x7 - Set end page to 7

	uint8_t command_data[12] = { 0x80, 0x21, 0x80, 0x0, 0x80, 127, 0x80, 0x22,
			0x80, 0x00, 0x80, 0x07 };

	// Create an array to hold the data to send to the display + 1 (byte to indicate data transmission, which is 0x40)
	uint8_t data[1025] = { 0 };
	data[0] = 0x40;

	// For each of the columns 0 to 127
	for (volatile uint8_t x = 0; x < 128; x++) {
		// For each of the pages, 0 to 7
		for (volatile uint8_t y = 0; y < 8; y++) {
			//retrieve the current page from the 2d array, and write that into the 1d array for transmission
			uint8_t current_page = SSD1306_FrameBufferPages[x][y];

			//8*128 + 1 = 1025
			//we skip the first byte as that is 0x40, to declare data transmission
			data[((y * 128) + x) + 1] = (uint8_t) current_page;
		}
	}

	I2C_SSD1306_Screen_Transmit((uint16_t) 12, command_data);
	I2C_SSD1306_Screen_Transmit((uint16_t) 1025, data);

	return;
}

void I2C_SSD1306_Screen_Transmit(uint16_t data_len, uint8_t *data) {
	HAL_I2C_Master_Transmit(hi2c, ((uint16_t) 0x3c) << 1, data, data_len,
			100);
}
