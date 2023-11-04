/* 	LCD Library
	STM32Fxx - HAL STM32CubeIDE
	Author : Fattah .A
	Electronics Engineering | State Polytechnic of Malang
	26 Jan. 2021
*/

#include "LCDI2C.h"
#include "stm32f4xx_hal.h"

#define i2c_perif 	 hi2c1		//Change this line based on your I2C
extern I2C_HandleTypeDef i2c_perif;		
#define i2c_handler	&i2c_perif

uint8_t ADDR;

void LCD_initID(uint8_t _ADDR) {
	ADDR = _ADDR;
}

void LCD_sendCMD(char cmd) {
	char _U, _L;
	uint8_t _T[4];
	_U = (cmd&0xf0);
	_L = ((cmd<<4)&0xf0);
	_T[0] = _U|0x0C;
	_T[1] = _U|0x08;
	_T[2] = _L|0x0C;
	_T[3] = _L|0x08;
	HAL_I2C_Master_Transmit (i2c_handler, ADDR,(uint8_t *) _T, 4, 100);
}

void LCD_sendData(char data) {
	char _U, _L;
	uint8_t _T[4];
	_U = (data&0xf0);
	_L = ((data<<4)&0xf0);
	_T[0] = _U|0x0D;
	_T[1] = _U|0x09;
	_T[2] = _L|0x0D;
	_T[3] = _L|0x09;
	HAL_I2C_Master_Transmit (i2c_handler, ADDR,(uint8_t *) _T, 4, 100);
}

void LCD_Clear(void) {
	LCD_sendData (0x00);
	for (int i=0; i<100; i++) {
		LCD_sendData (' ');
	}
}

void LCD_Init(void) {
	HAL_Delay(50);
	LCD_sendCMD (0x30);
	HAL_Delay(5);
	LCD_sendCMD (0x30);
	HAL_Delay(1);
	LCD_sendCMD (0x30);
	HAL_Delay(10);
	LCD_sendCMD (0x20);
	HAL_Delay(10);

	LCD_sendCMD (0x28);
	HAL_Delay(1);
	LCD_sendCMD (0x08);
	HAL_Delay(1);
	LCD_sendCMD (0x01);
	HAL_Delay(1);
	HAL_Delay(1);
	LCD_sendCMD (0x06);
	HAL_Delay(1);
	LCD_sendCMD (0x0C);
}

void LCD_sendSTR(char *str) {
	while (*str) LCD_sendData (*str++);
}

void LCD_setCursor(uint8_t x, uint8_t y) {
	uint8_t cursor;
	switch (x) {
	case 0 :
		cursor = 0x00;
		break;

	case 1 :
		cursor = 0x40;
		break;

	case 2 :
		cursor = 0x14;
		break;

	case 3 :
		cursor = 0x54;
		break;
	}
	cursor += y;
	LCD_sendCMD(0x80|cursor);
}
