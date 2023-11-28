/*
 * LCD.c
 *
 *  Created on: Sep 27, 2023
 *      Author: hesham mohamed
 */

/* =========================================================================
 *                                  Includes
 * =========================================================================*/
#include "LCD.h"


int count=0;

/*
 * =========================================================================
 * 								Generic Functions
 * =========================================================================*/
void LCD_itoa(uint32_t Data,char String[])
{
	uint8_t Digit=0,Count;
	if(!Data)String[Digit++]='0';
	else for(;Data;Digit++,Data/=10)
		String[Digit]=(Data%10+'0');
	String[Digit]=0;
	for(Count=0;Count<(Digit>>1);Count++)
	{
		String[Count]^=String[Digit-Count-1];
		String[Digit-Count-1]^=String[Count];
		String[Count]^=String[Digit-Count-1];
	}
}

static void ftoa(float Data,char String[])
{
	uint8_t Digit,Count;
	uint16_t Real=(uint16_t)(Data-(uint16_t)Data)*100;
	for(Digit=0;(uint32_t)Real;Digit++,Real/=10)
		String[Digit]=((uint32_t)Real%10+'0');
	String[Digit++]='.';
	for(;(uint32_t)Data;Digit++,Data/=10)
		String[Digit]=((uint32_t)Data%10+'0');
	for(Count=0;Count<(Digit>>1);Count++)
	{
		String[Count]^=String[Digit-Count-1];
		String[Digit-Count-1]^=String[Count];
		String[Count]^=String[Digit-Count-1];
	}
}


void HAL_LCD_Set_Courser(uint8_t Y_Axis,uint8_t X_Axis)
{
	uint8_t Line_Select;
	switch (Y_Axis)
	{
	case 1:
		Line_Select=LCD_BEGIN_AT_FIRST_ROW;break;
	case 2:
		Line_Select=LCD_BEGIN_AT_SECOND_ROW;break;
	case 3:
		Line_Select=LCD_THIRD_LINE;break;
	case 4:
		Line_Select=LCD_FOURTH_LINE;break;
	default:break;
	}
	HAL_LCD_Send_Command(Line_Select+X_Axis);
}

void check_line()
{
	if(count == 17)
		HAL_LCD_Set_Courser(2,0);
	else if(count == 33)
		HAL_LCD_Set_Courser(3,0);
	else if(count ==49)
		HAL_LCD_Set_Courser(4,0);
	else if(count == 65)
	{
		LCD_clear_screen();
		HAL_LCD_Set_Courser(1,0);
		count = 0;
	}
}

void LCD_clear_screen(void)
{
	HAL_LCD_Send_Command(LCD_DISP_ON);
	HAL_LCD_Send_Command(LCD_CLEAR_SCREEN);
	HAL_LCD_Send_Command(LCD_ENTRY_INC_);
	LCD_delay_ms(5);
	count = 0;
}

void LCD_clear_screen_2(void)
{
	HAL_LCD_Send_Command(LCD_DISP_OFF);
	HAL_LCD_Init();
	HAL_LCD_Send_Command(LCD_DISP_ON);
	HAL_LCD_Send_Command(LCD_CLEAR_SCREEN);
	HAL_LCD_Send_Command(LCD_ENTRY_INC_);
	count = 0;
}


void ENABLE_cycle()
{
	MCAL_GPIO_WritePin(LCD_Control_Port, LCD_EN, GPIO_PIN_SET);
	LCD_delay_ms(5);
	MCAL_GPIO_WritePin(LCD_Control_Port, LCD_EN, GPIO_PIN_RESET);
	LCD_delay_ms(2);
}

void LCD_delay_ms(uint32_t time){
	uint32_t i, j;
	for (i = 0; i < time; i++)
		for (j = 0; j < 255; j++)
			;
}


/* =========================================================================
 * 								APIs Functions Definitions
 * =========================================================================*/


/*************************************************************************
 * @Fn			-HAL_LCD_Init
 * @brief 		-Initializes LCD states in the ref
 * @param [in]	-n1
 * @retval 		-n1
 **************************************************************************/
void HAL_LCD_Init(void)
{
#if LCD_MODE == Eight_Bit_Mode
	uint32_t Pin_Counter,Pins[8]=LCD_Data_Pins;
#elif LCD_MODE == Four_Bit_Mode
	uint32_t Pin_Counter,Pins[4]=LCD_Data_Pins;
#endif

	GPIO_PinConfig_t  LCD_Pin;
	/*Initialize Control Pins*/
	LCD_Pin.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
	LCD_Pin.GPIO_Output_Speed = GPIO_SPEED_10M;
	LCD_Pin.GPIO_PinNumber = LCD_RS;
	MCAL_GPIO_Init(LCD_Control_Port, &LCD_Pin);
	LCD_Pin.GPIO_PinNumber = LCD_RW;
	MCAL_GPIO_Init(LCD_Control_Port, &LCD_Pin);
	LCD_Pin.GPIO_PinNumber = LCD_EN;
	MCAL_GPIO_Init(LCD_Control_Port, &LCD_Pin);
	MCAL_GPIO_WritePin(LCD_Control_Port, LCD_EN, GPIO_PIN_RESET);

#if LCD_MODE == Eight_Bit_Mode
	/*Initialize Data Pins*/
	for(Pin_Counter=0;Pin_Counter<8;Pin_Counter++)
	{
		LCD_Pin.GPIO_PinNumber=Pins[Pin_Counter];
		MCAL_GPIO_Init(LCD_Data_Port, &LCD_Pin);
	}
	LCD_delay_ms(40);
	/* Configure LCD */
	HAL_LCD_Send_Command(LCD_FUNCTION_8BIT_2LINES);
	HAL_LCD_Send_Command(LCD_DISP_ON_CURSOR_BLINK);
	HAL_LCD_Send_Command(LCD_ENTRY_INC_);
#elif LCD_MODE == Four_Bit_Mode
	/*Initialize Data Pins*/
	for(Pin_Counter=0;Pin_Counter<4;Pin_Counter++)
	{
		LCD_Pin.GPIO_PinNumber=Pins[Pin_Counter];
		MCAL_GPIO_Init(LCD_Data_Port, &LCD_Pin);
	}
	LCD_delay_ms(40);
	/* Configure LCD */
	HAL_LCD_Send_Command(LCD_RETURN_HOME);
	HAL_LCD_Send_Command(LCD_FUNCTION_4BIT_2LINES);
	HAL_LCD_Send_Command(LCD_DISP_ON_CURSOR_BLINK);
	HAL_LCD_Send_Command(LCD_ENTRY_INC_);
#else
#warning (" Wrong Configuration !!")
#endif
}


/*************************************************************************
 * @Fn			-HAL_LCD_Send_Command
 * @brief 		-Send Command to LCD
 * @param [in]	-command
 * @retval 		-n1
 **************************************************************************/
void HAL_LCD_Send_Command(uint8_t Command)
{
#if LCD_MODE == Eight_Bit_Mode
	uint32_t Pin_Counter,Pins[8]=LCD_Data_Pins;
#elif LCD_MODE == Four_Bit_Mode
	uint32_t Pin_Counter,Pins[4]=LCD_Data_Pins;
#endif
	// Turn off RS & RW
	MCAL_GPIO_WritePin(LCD_Control_Port, LCD_RW, GPIO_PIN_RESET);
	MCAL_GPIO_WritePin(LCD_Control_Port, LCD_RS, GPIO_PIN_RESET);
#if LCD_MODE == Eight_Bit_Mode
	/*Write Command*/
	for(Pin_Counter=0;Pin_Counter<8;Pin_Counter++)
		MCAL_GPIO_WritePin(LCD_Data_Port, Pins[Pin_Counter], ((Command>>Pin_Counter)&1));
#elif LCD_MODE == Four_Bit_Mode
	/*Send Hi Nibble*/
	for(Pin_Counter=0;Pin_Counter<4;Pin_Counter++)
		MCAL_GPIO_WritePin(LCD_Data_Port, Pins[Pin_Counter], ((Command>>Pin_Counter+4)&1));
	/*Enable LCD*/
	ENABLE_cycle();
	/*Send Low Nibble*/
	for(Pin_Counter=0;Pin_Counter<4;Pin_Counter++)
		MCAL_GPIO_WritePin(LCD_Data_Port, Pins[Pin_Counter], ((Command>>Pin_Counter)&1));
#endif
	/*Enable LCD*/
	ENABLE_cycle();
}

/*************************************************************************
 * @Fn			-HAL_LCD_Send_Data
 * @brief 		-Send data to LCD
 * @param [in]	-data
 * @retval 		-n1
 **************************************************************************/
void HAL_LCD_Send_Data(uint8_t Data)
{
#if LCD_MODE == Eight_Bit_Mode
	uint32_t Pin_Counter,Pins[8]=LCD_Data_Pins;
#elif LCD_MODE == Four_Bit_Mode
	uint32_t Pin_Counter,Pins[4]=LCD_Data_Pins;
#endif
	/*check line*/
	count++;
	check_line();
	LCD_delay_ms(10);
	// Turn on RS & off RW
	MCAL_GPIO_WritePin(LCD_Control_Port, LCD_RW, GPIO_PIN_RESET);
	MCAL_GPIO_WritePin(LCD_Control_Port, LCD_RS, GPIO_PIN_SET);

#if LCD_MODE == Eight_Bit_Mode
	/*Write Data*/
	for(Pin_Counter=0;Pin_Counter<8;Pin_Counter++)
		MCAL_GPIO_WritePin(LCD_Data_Port, Pins[Pin_Counter], ((Data>>Pin_Counter)&1));
#elif LCD_MODE == Four_Bit_Mode
	/*Send Hi Nibble*/
	for(Pin_Counter=0;Pin_Counter<4;Pin_Counter++)
		MCAL_GPIO_WritePin(LCD_Data_Port, Pins[Pin_Counter], ((Data>>Pin_Counter+4)&1));
	/*Enable LCD*/
	ENABLE_cycle();
	/*Send Low Nibble*/
	for(Pin_Counter=0;Pin_Counter<4;Pin_Counter++)
		MCAL_GPIO_WritePin(LCD_Data_Port, Pins[Pin_Counter], ((Data>>Pin_Counter)&1));
#endif
	/*Enable LCD*/
	ENABLE_cycle();
}


/*************************************************************************
 * @Fn			-HAL_LCD_Send_Float
 * @brief 		-send float number to LCD
 * @param [in]	-data
 * @retval 		-n1
 **************************************************************************/
void HAL_LCD_Send_Float(float Data)
{
	char String[8];
	ftoa(Data,String);
	HAL_LCD_Send_String(String);
}


/*************************************************************************
 * @Fn			-HAL_LCD_Send_Number
 * @brief 		-send Integer number to LCD
 * @param [in]	-data
 * @retval 		-n1
 **************************************************************************/
void HAL_LCD_Send_Number(uint32_t Data)
{
	char String[8];
	LCD_itoa(Data,String);
	HAL_LCD_Send_String(String);
}


/*************************************************************************
 * @Fn			-HAL_LCD_Send_String
 * @brief 		-send string number to LCD
 * @param [in]	-data
 * @retval 		-n1
 **************************************************************************/
void HAL_LCD_Send_String(char String[])
{
	while(*String)
	{
		HAL_LCD_Send_Data(*String);
		String++;
	}
}

