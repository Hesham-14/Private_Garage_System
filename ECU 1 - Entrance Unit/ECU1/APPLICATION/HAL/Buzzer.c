/*
 * Buzzer.c
 *
 * Created: 10/23/2023 9:43:41 PM
 *  Author: hesham mohamed
 */ 

/*****************************************
-----------     INCLUDES     -------------
 *****************************************/
#include "Buzzer.h"

/*****************************************
-----------    Functions     -------------
 *****************************************/
void Buzzer_Init(void)
{
	/* Configure Buzzer pin as output */
	GPIO_PinConfig_t pinCfg;
	pinCfg.GPIO_PinNumber = Buzzer_Pin;
	pinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
	pinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
	MCAL_GPIO_Init(Buzzer_Port, &pinCfg);
}

void Buzzer_Start(void)
{
	/* Start alarm */
	MCAL_GPIO_WritePin(Buzzer_Port, Buzzer_Pin, GPIO_PIN_SET);
	delay_ms(2000);

	/* Stop alarm */
	MCAL_GPIO_WritePin(Buzzer_Port, Buzzer_Pin, GPIO_PIN_RESET);
}

