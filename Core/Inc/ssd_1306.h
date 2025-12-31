/*
 * ssd_1306_drivers.h
 *
 *  Created on: 16 Dec 2025
 *      Author: Vanessa
 */

#ifndef INC_SSD_1306_H_
#define INC_SSD_1306_H_

void I2C_SSD1306_Screen_Init(I2C_HandleTypeDef *hi2c);
void I2C_SSD1306_Update_Whole_Display(uint8_t SSD1306_FrameBufferPages[128][8]);
void I2C_SSD1306_Screen_Transmit(uint16_t data_len, uint8_t *data);

#endif /* INC_SSD_1306_H_ */
