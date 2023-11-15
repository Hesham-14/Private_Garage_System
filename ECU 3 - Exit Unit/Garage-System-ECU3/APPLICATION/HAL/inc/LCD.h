/*
 * LCD.h
 *
 *  Created on: Sep 27, 2023
 *      Author: hesham mohamed
 */

#ifndef INC_LCD_H_
#define INC_LCD_H_


/*****************************************
-------------   Includes   -------------
*****************************************/
//#include "../../MCAL/inc/STM32F103C6_GPIO_Driver.h"
//#include "../../MCAL/inc/STM32F103x6.h"

#include "STM32F103C6_GPIO_Driver.h"
#include "STM32F103x6.h"



/*****************************************
-------   Macros Configuration   --------
*****************************************/
#define addString(x) ((uint8_t *)x)

//commands
#define SET_BIT(reg,bit) (reg |= (1<< bit))
#define CLR_BIT(reg,bit) (reg &= ~(1<< bit))
#define TOG_BIT(reg,bit) (reg ^= (1<< bit))

//define Mode
#define Eight_Bit_Mode                   (1)
#define Four_Bit_Mode                    (2)

//LCD Commands
#define LCD_FUNCTION_8BIT_2LINES	(0x38)
#define LCD_FUNCTION_4BIT_2LINES	(0x28)
#define LCD_MOVE_DISP_RIGHT			(0x1C)
#define LCD_MOVE_DISP_LIFT			(0x18)
#define LCD_MOVE_CURSOR_RIGHT   	(0x14)
#define LCD_MOVE_CURSOR_LEFT 	  	(0x10)
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
#define LCD_BEGIN_AT_THIRD_ROW		(0x90)
#define LCD_BEGIN_AT_FOURTH_ROW		(0xD0)
#define LCD_CLEAR_SCREEN			(0x01)
#define LCD_ENTRY_MODE				(0x06)
#define LCD_RETURN_HOME				(0x02)
#define LCD_THIRD_LINE		  		(0x90)
#define LCD_FOURTH_LINE		  		(0xD0)


/*****************************************
---------   Prebuild Configure   ---------
*****************************************/
#define LCD_MODE                         (Four_Bit_Mode)
#define LCD_Data_Port                    (GPIOA)
#define LCD_Data_Pins                    {GPIO_PIN_12,GPIO_PIN_13,GPIO_PIN_14,GPIO_PIN_15}
#define LCD_Control_Port                 (GPIOB)
#define LCD_RS                           (GPIO_PIN_5)
#define LCD_RW                           (GPIO_PIN_6)
#define LCD_EN                           (GPIO_PIN_7)


/*****************************************
----------------- APIs  -----------------
*****************************************/
void HAL_LCD_Init(void);
void HAL_LCD_Send_Data(uint8_t Data);
void HAL_LCD_Send_Float(float Data);
void HAL_LCD_Send_Number(uint32_t Data);
void HAL_LCD_Send_String(char String[]);
void HAL_LCD_Set_Courser(uint8_t Y_Axis,uint8_t X_Axis);
void LCD_clear_screen(void);
void LCD_clear_screen_2(void);




#endif /* INC_LCD_H_ */


