
/*
 * Gate.h
 *
 * Created: 10/23/2023 12:08:25 AM
 *  Author: hesham mohamed
 */ 

#ifndef GATE_H_
#define GATE_H_

/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "avr/io.h"
#include "avr/interrupt.h"
#include "Timer.h"
#include "Macros.h"
#include "Standard_Types.h"


/*************************************
---------------  APIs  ---------------
***************************************/
void Gate_Init(void);
void Gate_Open(void);
void Gate_Close(void);

#endif /* GATE_H_ */