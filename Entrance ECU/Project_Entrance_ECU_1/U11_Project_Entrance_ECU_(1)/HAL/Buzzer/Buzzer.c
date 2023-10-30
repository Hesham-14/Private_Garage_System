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
	Set_Bit(Buzzer_DDR, Buzzer_Pin);
}

void Buzzer_Start(void)
{
	/* Start alarm */
	Set_Bit(Buzzer_Port, Buzzer_Pin);
    delay_ms(3000);
	
	/* Stop alarm */
    Clear_Bit(Buzzer_Port, Buzzer_Pin);
}

