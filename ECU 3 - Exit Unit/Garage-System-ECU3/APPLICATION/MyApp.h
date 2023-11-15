/*
 * MyApp.h
 *
 *  Created on: Nov 6, 2023
 *      Author: hesham mohamed
 */

#ifndef MYAPP_H_
#define MYAPP_H_

/*****************************************
-------------   Includes   -------------
*****************************************/
#include "STM32F103x6.h"
#include "STM32F103C6_USART_Driver.h"
#include "STM32F103C6_GPIO_Driver.h"
#include "STM32F103C6_RCC_Driver.h"
#include "STM32F103C6_EXTI_Driver.h"
#include "STM32F103C6_SPI_Driver.h"
#include "Buzzer.h"
#include "Gate.h"
#include "Motion.h"
#include "LCD.h"
#include "Keypad.h"




/*****************************************
---------   Prebuild Configure   ---------
*****************************************/
/*--------- SPI Macros ---------*/
//#define MCU_Act_As_Master
#define MCU_Act_As_Slave

/*--------- SPI SS Pins ---------*/
#define SPI_MASTER_ACK				'&'
#define SPI_VALID_ACK				'T'
#define SPI_INVALID_ACK				'F'
#define SS_Port						(GPIOA)
#define Entrance_SS					(GPIO_PIN_0)
#define Exit_SS						(GPIO_PIN_1)

/*--------- EXTI Pins ---------*/
#define EXTI_Pin					(GPIO_PIN_8)
#define EXTI_Port					(GPIOA)
#define Start_Trigger_Pin			(GPIO_PIN_0)
#define Start_Trigger_PORT			(GPIOB)

/*--------- LED Pins ---------*/
#define VALID_LED						(GPIO_PIN_12)
#define INVALID_LED						(GPIO_PIN_13)
#define LED_PORT						(GPIOB)

/*--------- User  Macros ---------*/
#define MAX_SLOTS_IN_GARAGE				3
#define ID_SIZE							7
#define NAME_MAX_SIZE					10
#define UART_TERMINATE_CHAR				13


/*****************************************
---------   Global Variables   ---------
*****************************************/



/*********************************************
---------   User type definitions  ---------
**********************************************/
#define addString(x) ((uint8_t *)x)


/*****************************************
----------------- APIs  -----------------
*****************************************/
void ECU3_Dashboard_APP_SETUP(void);
void ECU3_Dashboard_APP_RUN(void);
static void ECU3_UART_IRQ_CallBack(void);
static void ECU3_SPI_IRQ_CallBack(struct S_IRQ_SRC irq_src);


#endif /* MYAPP_H_ */
