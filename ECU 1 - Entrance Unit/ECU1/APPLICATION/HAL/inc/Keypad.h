/*
 * Keypad.h
 *
 *  Created on: May 6, 2023
 *      Author: hesham mohamed
 */

#ifndef INC_KEYPAD_H_
#define INC_KEYPAD_H_

/*****************************************
----------   Include Files    ------------
*****************************************/
//#include "../../MCAL/inc/STM32F103C6_GPIO_Driver.h"

#include "STM32F103C6_GPIO_Driver.h"


/*****************************************
---------   Prebuild Configure   ---------
*****************************************/
//define PORT
#define Keypad_Port						(GPIOB)
//Select Pins for Keypad Rows
#define Keypad_Rows_Pins				{GPIO_PIN_0,GPIO_PIN_1,GPIO_PIN_2,GPIO_PIN_3}
//Select Pins for Keypad Columns
#define Keypad_Columns_Pins				{GPIO_PIN_4,GPIO_PIN_5,GPIO_PIN_6}
//Select Keypad Layout
#define Keypad_Layout                   {{'*','0','#'},\
					                    {'7','8','9'}, \
					                    {'4','5','6'}, \
					                    {'1','2','3'}} \


/*****************************************
---------    Configure Macros    ---------
*****************************************/
//commands
#define SET_BIT(reg,bit) (reg |= (1<< bit))
#define CLR_BIT(reg,bit) (reg &= ~(1<< bit))
#define TOG_BIT(reg,bit) (reg ^= (1<< bit))

#define Keypad_Not_Pressed			('X')
#define Keypad_ROWS_SIZE			(4)
#define Keypad_COLUMNS_SIZE			(3)

/*****************************************
--------------    APIs    ---------------
*****************************************/
void 	HAL_keypad_init(void);
char 	HAL_keypad_get_key(void);
uint8_t HAL_Keypad_Wait_For_Input(void);

#endif /* INC_KEYPAD_H_ */
