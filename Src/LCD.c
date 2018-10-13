#include "main.h"
#include "stm32f4xx_hal.h"

#include "LCD.h"

LCD_DEF lcd;
uint8_t CMD_GAMMA[] = { 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F };

void LCD_WRITE_DATA8(uint8_t data) {
	LCD_CS_RESET;
	LCD_DC_SET;
	HAL_SPI_Transmit(&LCD_SPI, (uint8_t*) &data, 1, 1000);
	LCD_CS_SET;
}

void LCD_WRITE_DATA(uint16_t data) {
	uint8_t transmit[2] = { data >> 8, data };
	LCD_CS_RESET;
	LCD_DC_SET;
	HAL_SPI_Transmit(&LCD_SPI, transmit, 2, 1000);
	LCD_CS_SET;
}

void LCD_WRITE_STREAM(uint8_t *data, uint16_t size) {
	LCD_CS_RESET;
	LCD_DC_SET;
	HAL_SPI_Transmit(&LCD_SPI, data, size, 1000);
	LCD_CS_SET;
}

void LCD_WRITE_REG(uint8_t addr) {
	LCD_CS_RESET;
	LCD_DC_RESET;
	HAL_SPI_Transmit(&LCD_SPI, (uint8_t*) &addr, 1, 0);
	LCD_CS_SET;
}

void LCD_WRITE_REG_DATA(uint8_t addr, uint16_t val) {
	LCD_WRITE_REG(addr);
	LCD_WRITE_DATA(val);
}

void LCD_PREPARE_WRITE_RAM() {
	LCD_WRITE_REG(lcd.cmd_gram);
}

void LCD_SET_CURSOR(uint16_t x, uint16_t y) {
	LCD_WRITE_REG(lcd.cmd_x);
	LCD_WRITE_DATA(x);
	LCD_WRITE_REG(lcd.cmd_y);
	LCD_WRITE_DATA(y);
}

void LCD_DRAW_POINT(uint16_t x, uint16_t y) {
	LCD_SET_CURSOR(x, y);
	LCD_PREPARE_WRITE_RAM();
	LCD_WRITE_DATA(BRUSH_COLOR);
}

void LCD_ERASE_POINT(uint16_t x, uint16_t y) {
	LCD_SET_CURSOR(x, y);
	LCD_PREPARE_WRITE_RAM();
	LCD_WRITE_DATA(BACKGROUND_COLOR);
}

void LCD_INIT() {
	LCD_LED_RESET;
	HAL_Delay(50);
	LCD_LED_SET;
	HAL_Delay(50);

	LCD_RS_RESET;
	HAL_Delay(20);
	LCD_RS_SET;
	HAL_Delay(20);

	lcd.width = LCD_WIDTH;
	lcd.height = LCD_HEIGHT;
	lcd.cmd_gram = 0x2C;
	lcd.cmd_x = 0x2A;
	lcd.cmd_y = 0x2B;

	LCD_WRITE_REG(0xCB);
    LCD_WRITE_DATA8(0x39);
    LCD_WRITE_DATA8(0x2C);
    LCD_WRITE_DATA8(0x00);
    LCD_WRITE_DATA8(0x34);
    LCD_WRITE_DATA8(0x02);

    LCD_WRITE_REG(0xCF);
    LCD_WRITE_DATA8(0x00);
    LCD_WRITE_DATA8(0XC1);
    LCD_WRITE_DATA8(0X30);

    LCD_WRITE_REG(0xE8);
    LCD_WRITE_DATA8(0x85);
    LCD_WRITE_DATA8(0x00);
    LCD_WRITE_DATA8(0x78);

    LCD_WRITE_REG(0xEA);
    LCD_WRITE_DATA8(0x00);
    LCD_WRITE_DATA8(0x00);

    LCD_WRITE_REG(0xED);
    LCD_WRITE_DATA8(0x64);
    LCD_WRITE_DATA8(0x03);
    LCD_WRITE_DATA8(0X12);
    LCD_WRITE_DATA8(0X81);

    LCD_WRITE_REG(0xF7);
    LCD_WRITE_DATA8(0x20);

    LCD_WRITE_REG(0xC0);    //Power control
    LCD_WRITE_DATA8(0x23);   //VRH[5:0]

    LCD_WRITE_REG(0xC1);    //Power control
    LCD_WRITE_DATA8(0x10);   //SAP[2:0];BT[3:0]

    LCD_WRITE_REG(0xC5);    //VCM control
    LCD_WRITE_DATA8(0x3e); //对比度调节
    LCD_WRITE_DATA8(0x28);

    LCD_WRITE_REG(0xC7);    //VCM control2
    LCD_WRITE_DATA8(0x86);  //--

    LCD_WRITE_REG(0x36);    // Memory Access Control
    LCD_WRITE_DATA8(0x48); //C8	   //48 68竖屏//28 E8 横屏

    LCD_WRITE_REG(0x3A);
    LCD_WRITE_DATA8(0x55);

    LCD_WRITE_REG(0xB1);
    LCD_WRITE_DATA8(0x00);
    LCD_WRITE_DATA8(0x18);

    LCD_WRITE_REG(0xB6);    // Display Function Control
    LCD_WRITE_DATA8(0x08);
    LCD_WRITE_DATA8(0x82);
    LCD_WRITE_DATA8(0x27);

    LCD_WRITE_REG(0xF2);    // 3Gamma Function Disable
    LCD_WRITE_DATA8(0x00);

    LCD_WRITE_REG(0x26);    //Gamma curve selected
    LCD_WRITE_DATA8(0x01);

    LCD_WRITE_REG(0xE0);    //Set Gamma
    LCD_WRITE_STREAM(CMD_GAMMA, CMD_GAMMA_SIZE);

    LCD_WRITE_REG(0XE1);    //Set Gamma
    LCD_WRITE_STREAM(CMD_GAMMA, CMD_GAMMA_SIZE);


    LCD_WRITE_REG(0x11);    //Exit Sleep
    HAL_Delay(120);

    LCD_WRITE_REG(0x29);    //Display on
    LCD_WRITE_REG(0x2c);

    LCD_DRAW_CLEAR(BACKGROUND_COLOR);
}

void LCD_DRAW_CLEAR(uint16_t color) {
	uint32_t index;
	uint32_t total_point = lcd.width * lcd.height;

	LCD_SET_CURSOR(0x00, 0x0000);
	LCD_PREPARE_WRITE_RAM();
	for(index = 0; index < total_point; index++) {
		LCD_WRITE_DATA(color);
	}
}
