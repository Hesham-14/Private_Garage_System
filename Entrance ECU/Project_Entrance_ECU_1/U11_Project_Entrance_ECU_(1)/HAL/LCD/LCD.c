/*
 * LCD.c
 *
 * Created: 4/26/2023 9:35:49 AM
 *  Author: hesham mohamed
 */ 
#include "LCD.h"


void ENABLE_cycle(void)
{
	SET_BIT(LCD_CTRL, EN_SWITCH);
	_delay_ms(50);
	CLR_BIT(LCD_CTRL, EN_SWITCH);
}

void LCD_isbusy(void)
{
	//1. disable ports as output
	DataDir_LCD_DATA = 0x00;
	//2. to read data --> set RW = 1
	SET_BIT(LCD_CTRL, RW_SWITCH);
	//3. clear RS
	CLR_BIT(LCD_CTRL, RS_SWITCH);
	//4. enable cycle
	ENABLE_cycle();
	//5. enable ports as output
	DataDir_LCD_DATA = 0xFF;
	//6. clear RS
	CLR_BIT(LCD_CTRL, RS_SWITCH);
}

void LCD_clear_screen(void)
{
	LCD_command(LCD_CLEAR_SCREEN);
}

int LCD_GOTO(uint8_t  line, uint8_t position)
{
	//false case
	if(position > 16 || position < 0)
	return 0;

	if(line == 1)
	LCD_command(LCD_BEGIN_AT_FIRST_ROW + position);
	else if(line == 2)
	LCD_command(LCD_BEGIN_AT_SECOND_ROW + position);
	return 1;
}

void LCD_init(void)
{
	//0. waiting power ON
	_delay_ms(20);
	//1. set LCD data ports (PA0:PA7) as output
	DataDir_LCD_DATA = 0xFF;
	//2. set LCD CTRL ports (PC0:PC2) as output, then reset
	DataDir_LCD_CTRL = 0b1110;
	LCD_CTRL &= ~(0b1110);
	//waiting 
	_delay_ms(15);

	//3. initialize LCD
	#ifdef _8_BIT_MODE
	LCD_command(LCD_FUNCTION_8BIT_2LINES);
	#endif
	#ifdef _4_BIT_MODE
	LCD_command(LCD_RETURN_HOME);
	LCD_command(LCD_FUNCTION_4BIT_2LINES);
	#endif
	
	//4. clear LCD
	LCD_command(LCD_CLEAR_SCREEN);
	
	//5. Return home
	LCD_command(LCD_RETURN_HOME);
	
	//6. Entry mode
	LCD_command(LCD_ENTRY_MODE);
	
	//7. start at first row
	LCD_command(LCD_BEGIN_AT_FIRST_ROW);
	
	//8. cursor blink
	LCD_command(LCD_DISP_ON_CURSOR_BLINK);
}

void LCD_command(uint8_t cmd)
{
	#ifdef _8_BIT_MODE
	//0. check busy
	LCD_isbusy();
	//1. copy data
	LCD_DATA = cmd;
	//2. clear RS & RW
	CLR_BIT(LCD_CTRL, RS_SWITCH);
	CLR_BIT(LCD_CTRL, RW_SWITCH);
	_delay_ms(1);
	//3. enable cycle
	ENABLE_cycle();
	#endif
	
	#ifdef _4_BIT_MODE
	//0. check busy
	LCD_isbusy();
	//1. copy first line data, then clear RS & RW
	LCD_DATA = (LCD_DATA & 0x0F) | (cmd & 0xF0);
	CLR_BIT(LCD_CTRL, RS_SWITCH);
	CLR_BIT(LCD_CTRL, RW_SWITCH);
	_delay_ms(1);
	//2. copy first line data, then clear RS & RW
	LCD_DATA = (LCD_DATA & 0x0F) | (cmd << 4);
	CLR_BIT(LCD_CTRL, RS_SWITCH);
	CLR_BIT(LCD_CTRL, RW_SWITCH);
	_delay_ms(1);
	//3. enable cycle
	ENABLE_cycle();
	#endif
}

void LCD_write_char(uint8_t data)
{
	#ifdef _8_BIT_MODE
	//0. check busy
	LCD_isbusy();
	//1. copy data
	LCD_DATA = data;
	//2. set RS & clear RW
	SET_BIT(LCD_CTRL, RS_SWITCH);
	CLR_BIT(LCD_CTRL, RW_SWITCH);
	_delay_ms(1);
	//3. enable cycle
	ENABLE_cycle();
	#endif
	
	#ifdef _4_BIT_MODE
	//0. check busy
	LCD_isbusy();
	//1. copy first line data, then set RS & clear RW
	LCD_DATA = (LCD_DATA & 0x0F) | (data & 0xF0);
	SET_BIT(LCD_CTRL, RS_SWITCH);
	CLR_BIT(LCD_CTRL, RW_SWITCH);
	_delay_ms(1);
	//2. copy first line data, then set RS & clear RW
	LCD_DATA = (LCD_DATA & 0x0F) | (data << 4);	
	SET_BIT(LCD_CTRL, RS_SWITCH);
	CLR_BIT(LCD_CTRL, RW_SWITCH);
	_delay_ms(1);
	//3. enable cycle
	ENABLE_cycle();
	#endif
}

void LCD_write_string(uint8_t* data)
{
	int count=0;
	while(*data > 0)
	{
		count++;
		LCD_write_char(*data++);
		
		//check line
		if(count == 16)
		LCD_GOTO(2,0);
		else if(count == 32)
		{
			LCD_clear_screen();
			LCD_GOTO(2,0);
			count = 0;
		}		
	}
}

void LCD_display_number (int Number )
{

	char  str[7];

	sprintf(str,"%d",Number);	// Adjust the formatting to your liking.

	LCD_write_string(str);

}