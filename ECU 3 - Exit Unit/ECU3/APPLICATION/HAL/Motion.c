
/*
 * Motion.c
 *
 * Created: 10/23/2023 10:20:06 PM
 *  Author: hesham mohamed
 */ 
/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "Motion.h"

/*****************************************
-----------    Functions     -------------
*****************************************/
void Motion_Sensor_Init(void)
{
	/* Initialize pin as input */
	GPIO_PinConfig_t pinCfg;
	pinCfg.GPIO_PinNumber = Motion_Sensor_Pin;
	pinCfg.GPIO_Mode = GPIO_MODE_INPUT_FLO;
	MCAL_GPIO_Init(Motion_Sensor_Port, &pinCfg);
}

u8 Motion_Sensor_Get_State(void)
{  
	u8 LOC_retval;
	LOC_retval = MCAL_GPIO_ReadPin(Motion_Sensor_Port, Motion_Sensor_Pin);

    return LOC_retval;
}

