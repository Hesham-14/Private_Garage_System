/*
 * LCD.h
 *
 * Created: 4/26/2023 9:35:58 AM
 *  Author: hesham mohamed
 */ 


#ifndef LCD_H_
#define LCD_H_


#include <avr/io.h>
#include <util/delay.h>

//commands
#define SET_BIT(reg,bit) (reg |= (1<< bit))
#define CLR_BIT(reg,bit) (reg &= ~(1<< bit))
#define TOG_BIT(reg,bit) (reg ^= (1<< bit))

#define LCD_DATA PORTA			//selected as data port
#define DataDir_LCD_DATA DDRA	//enable as output port
#define LCD_CTRL PORTC
#define DataDir_LCD_CTRL DDRC
#define RS_SWITCH 1
#define RW_SWITCH 2
#define EN_SWITCH 3

//MODES
#define _8_BIT_MODE
//#define _4_BIT_MODE

//LCD Commands
#define LCD_FUNCTION_8BIT_2LINES	(0x38)
#define LCD_FUNCTION_4BIT_2LINES	(0x28)
#define LCD_MOVE_DISP_RIGHT			(0x1C)
#define LCD_MOVE_DISP_LIFT			(0x18)
#define LCD_MOVE_DISP_RIGHT			(0x14)
#define LCD_MOVE_DISP_LIFT			(0x10)
#define LCD_DISP_OFF				(0x08)
#define LCD_DISP_ON_CURSOR			(0x0E)
#define LCD_DISP_ON_CURSOR_BLINK	(0x0F)
#define LCD_DISP_ON_BLINK			(0x0D)
#define LCD_DISP_ON					(0x0C)
#define LCD_ENTRY_DEC				(0x04)
#define LCD_ENTRY_DEC_SHIFT			(0x05)
#define LCD_ENTRY_INC_				(0x06)
#define LCD_ENTRY_INC_SHIFT			(0x07)
#define LCD_BEGIN_AT_FIRST_ROW		(0x80)
#define LCD_BEGIN_AT_SECOND_ROW		(0xC0)
#define LCD_CLEAR_SCREEN			(0x01)
#define LCD_ENTRY_MODE				(0x06)
#define LCD_RETURN_HOME				(0x02)

//used functions
void LCD_init(void);
void LCD_command(uint8_t cmd);
void LCD_write_char(uint8_t data);
void LCD_write_string(uint8_t* data);
void LCD_isbusy(void);
void LCD_clear_screen(void);
int LCD_GOTO(uint8_t  line, uint8_t position);
void LCD_display_number (int Number );

#endif /* LCD_H_ */