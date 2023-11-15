/*
 * Keypad.c
 *
 *  Created on: May 6, 2023
 *      Author: hesham mohamed
 */

/*****************************************
-----------     INCLUDES     -------------
 *****************************************/
#include "Keypad.h"


/*****************************************
--------    GLOBAL VARIABLES     ---------
 *****************************************/
static uint8_t Keypad_Array[Keypad_ROWS_SIZE][Keypad_COLUMNS_SIZE]=Keypad_Layout;
static uint8_t Keypad_Columns[Keypad_COLUMNS_SIZE]=Keypad_Columns_Pins;
static uint8_t Keypad_Rows[Keypad_ROWS_SIZE]=Keypad_Rows_Pins;

/*****************************************
--------------    APIs    ---------------
 *****************************************/
/*************************************************************************
 * @Fn			-HAL_keypad_init
 * @brief 		-Initializes Keypad Pins
 * @param [in]	-none
 * @retval 		-none
 **************************************************************************/
void HAL_keypad_init(void)
{
	uint32_t Pin_Counter;
	GPIO_PinConfig_t Keypad_Pin;

	/*Configure Rows as Input Pins*/
	Keypad_Pin.GPIO_Mode = GPIO_MODE_INPUT_PU;
	for(Pin_Counter=0;Pin_Counter<Keypad_ROWS_SIZE;Pin_Counter++)
	{
		Keypad_Pin.GPIO_PinNumber=Keypad_Rows[Pin_Counter];
		MCAL_GPIO_Init(Keypad_Port, &Keypad_Pin);
	}

	/*Configure Columns as Output Pins*/
	Keypad_Pin.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
	Keypad_Pin.GPIO_Output_Speed = GPIO_SPEED_10M;
	for(Pin_Counter=0;Pin_Counter<Keypad_COLUMNS_SIZE;Pin_Counter++)
	{
		Keypad_Pin.GPIO_PinNumber=Keypad_Columns[Pin_Counter];
		MCAL_GPIO_Init(Keypad_Port, &Keypad_Pin);
		MCAL_GPIO_WritePin(Keypad_Port, Keypad_Columns[Pin_Counter], GPIO_PIN_SET);
	}
}

/*************************************************************************
 * @Fn			-HAL_keypad_get_key
 * @brief 		-Read pressed key from keypad
 * @param [in]	-none
 * @retval 		-keypad value
 **************************************************************************/
char 	HAL_keypad_get_key(void)
{
	uint8_t Column_Counter,Row_Counter,Keypad_Value=Keypad_Not_Pressed;
	for(Column_Counter=0;Column_Counter<Keypad_COLUMNS_SIZE;Column_Counter++)
	{
		MCAL_GPIO_WritePin(Keypad_Port, Keypad_Columns[Column_Counter], GPIO_PIN_RESET);
		for(Row_Counter=0;Row_Counter<Keypad_ROWS_SIZE;Row_Counter++)
		{
			if(!MCAL_GPIO_ReadPin(Keypad_Port, Keypad_Rows[Row_Counter]))
			{
				Keypad_Value=Keypad_Array[Row_Counter][Column_Counter];
				while (!MCAL_GPIO_ReadPin(Keypad_Port, Keypad_Rows[Row_Counter]));
				break;
			}
		}
		MCAL_GPIO_WritePin(Keypad_Port, Keypad_Columns[Column_Counter], GPIO_PIN_SET);
	}
	return Keypad_Value;
}

/*************************************************************************
 * @Fn			-HAL_Keypad_Wait_For_Input
 * @brief 		-Read pressed key from keypad
 * @param [in]	-none
 * @retval 		-keypad value
 **************************************************************************/
uint8_t HAL_Keypad_Wait_For_Input(void)
{
	uint8_t Input_Char;
	do {
		Input_Char=HAL_keypad_get_key();
	} while (Input_Char==Keypad_Not_Pressed);
	return Input_Char;
}







