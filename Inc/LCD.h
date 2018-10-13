/*
 * LCD.h
 *
 *  Created on: Oct 13, 2018
 *      Author: skyzh
 */

#ifndef LCD_H_
#define LCD_H_

typedef struct {
	uint16_t width;
	uint16_t height;
	uint16_t id;
	uint8_t	 cmd_gram;
	uint8_t  cmd_x;
	uint8_t  cmd_y;
} LCD_DEF;

typedef struct {
	uint16_t LCD_REG;
	uint16_t LCD_RAM;
} LCD_TypeDef;

#define LCD_BASE        ((u32)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

#define LCD_RS_RESET    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET)
#define LCD_RS_SET      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET)

#define LCD_DC_RESET    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET)
#define LCD_DC_SET      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET)

#define LCD_CS_RESET    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET)
#define LCD_CS_SET      HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET)

#define LCD_LED_RESET   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET)
#define LCD_LED_SET     HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET)

extern SPI_HandleTypeDef hspi1;
#define LCD_SPI hspi1

#define BRUSH_COLOR 0x0000
#define BACKGROUND_COLOR 0xFFFF

void LCD_WRITE_DATA8(uint8_t data);
void LCD_WRITE_DATA(uint16_t data);
void LCD_WRITE_REG(uint8_t val);
void LCD_WRITE_REG_DATA(uint8_t addr, uint16_t val);
void LCD_PREPARE_WRITE_RAM();
void LCD_SET_CURSOR(uint16_t x, uint16_t y);
void LCD_DRAW_POINT(uint16_t x, uint16_t y);
void LCD_INIT();
void LCD_DRAW_CLEAR(uint16_t color);

#define CMD_GAMMA_SIZE 15
#define LCD_WIDTH 240
#define LCD_HEIGHT 320


#endif /* LCD_H_ */
