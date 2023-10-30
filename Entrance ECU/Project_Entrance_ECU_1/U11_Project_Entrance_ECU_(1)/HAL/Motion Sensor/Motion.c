
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
	/* Initialize pin as input by clearing its DDRx*/
	Clear_Bit(Motion_Sensor_DDR, Motion_Sensor_Pin);
}

u8 Motion_Sensor_Get_State(void)
{  
	u8 LOC_retval;
	LOC_retval = Get_Bit(Motion_Sensor_Port, Motion_Sensor_Pin);
	//LOC_retval = Motion_Sensor_Port & (1 <<Motion_Sensor_Pin);
    return LOC_retval;
}

