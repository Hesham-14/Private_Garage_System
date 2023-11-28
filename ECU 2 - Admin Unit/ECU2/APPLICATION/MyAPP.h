/*
 * MyAPP.h
 *
 *  Created on: Oct 3, 2023
 *      Author: hesham mohamed
 */

#ifndef MYAPP_H_
#define MYAPP_H_


/*****************************************
-------------   Includes   -------------
*****************************************/
#include "STM32F103x6.h"
#include "STM32F103C6_GPIO_Driver.h"
#include "STM32F103C6_RCC_Driver.h"
#include "STM32F103C6_EXTI_Driver.h"
#include "STM32F103C6_USART_Driver.h"
#include "STM32F103C6_SPI_Driver.h"
#include "LCD.h"
#include "Keypad.h"


/*****************************************
---------   Prebuild Configure   ---------
*****************************************/
/*--------- SPI Macros ---------*/
#define ECU2_Act_As_Master
//#define ECU2_Act_As_Slave

/*--------- SPI SS Pins ---------*/
#define SPI_MASTER_ACK				'&'
#define SPI_VALID_ACK				'T'
#define SPI_INVALID_ACK				'F'
#define SS_Port						(GPIOA)
#define Entrance_SS					(GPIO_PIN_3)
#define Exit_SS						(GPIO_PIN_1)

/*--------- EXTI Pins ---------*/
#define Entrance_Pin				(EXTI2PA2)
#define Exit_Pin					(EXTI0PA0)
#define Start_Trigger_Pin			(GPIO_PIN_11)


/*--------- ADMIN Macros ---------*/
#define KEYPAD_BACK_SYMBOL			'#'
#define ID_SIZE						7
#define NAME_MAX_SIZE				10
#define UART_TERMINATE_CHAR			13
#define ADMIN1_NAME					"khaled"
#define ADMIN1_PASSWORD				"2468"
#define ADMIN2_NAME					"montaser"
#define ADMIN2_PASSWORD				"2023"

/*--------- User  Macros ---------*/
#define NUMBER_OF_ADMINS			2
#define NUMBER_OF_DRIVERS			5
#define MAX_SLOTS_IN_GARAGE			3
#define DRIVER1_NAME				"Jerry"
#define DRIVER1_ID					"10111213"
#define DRIVER2_NAME				"Jack"
#define DRIVER2_ID					"14151617"
#define DRIVER3_NAME				"Bee"
#define DRIVER3_ID					"18192021"





/*****************************************
---------   Global Variables   ---------
*****************************************/



/*********************************************
---------   User type definitions  ---------
**********************************************/



/*****************************************************
---------   Macros Configuration  Refrences  --------
******************************************************/
#define OPTION_ADD					'1'
#define OPTION_DELETE				'2'
#define OPTION_EDIT					'3'
#define OPTION_DELETE_USER			'1'
#define OPTION_DELETE_ALL			'2'

#define ADMIN_VALID_ACCESS			40	//random value
#define ADMIN_INVALID_ACCESS		50	//random value
#define SUCCESSFUL_OPERATION		'S'
#define FAILED_OPERATION			'F'

/*****************************************
----------------- APIs  -----------------
*****************************************/
void UART_IRQ_Callback(void);
void ECU2_ADMIN_Dashboard(void);
void ECU2_Dashboard_APP_SETUP(void);
void ECU2_Dashboard_APP_RUN(void);

#endif /* MYAPP_H_ */
