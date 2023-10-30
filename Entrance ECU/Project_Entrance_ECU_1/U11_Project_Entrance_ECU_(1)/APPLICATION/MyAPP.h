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
#define F_CPU 8000000UL
#include <avr/io.h>
#include <avr/iom32.h>
#include <util/delay.h>
#include <string.h>
#include "LCD.h"
#include "USART.h"
#include "SPI.h"
#include "Timer.h"
#include "Gate.h"
#include "Buzzer.h"
#include "Motion.h"


/*****************************************
---------   Pre-build Configure   --------
*****************************************/
/*--------- SPI Macros ---------*/
//#define MCU_SPI_Master
#define MCU_SPI_Slave

/*--------- SPI SS Pins ---------*/
#define SPI_TERMINAL_CHAR			13
#define SPI_MASTER_ACK				'&'
#define SPI_VALID_ACK				'T'
#define SPI_INVALID_ACK				'F'
#define Entrance_EXTI				(PB0)
#define Exit_EXTI					(PB0)
#define EXTI_DDR					(DDRB)
#define EXTI_Port					(PORTB)

/*--------- LED Pins ---------*/
#define VALID_LED						(PC6)
#define INVALID_LED						(PC7)
#define LED_DDR							(DDRC)
#define LED_PORT						(PORTC)

/*********************************************
---------   User type definitions  ---------
**********************************************/
typedef enum {
	Valid_Access,
	InValid_Access
}Access_Status_t;

/*****************************************************
---------   Macros Configuration References  --------
******************************************************/


/********* User  Macros *********/
#define MAX_SLOTS_IN_GARAGE				3
#define ID_SIZE							7
#define NAME_MAX_SIZE					10
#define UART_TERMINATE_CHAR				13
#define SUCCESSFUL_OPERATION			'S'
#define FAILED_OPERATION				'F'


/*****************************************
----------------- APIs  -----------------
*****************************************/
void ECU1_APP_SETUP(void);
void ECU1_APP_RUNNING(void);
void ECU1_APP_Valid_Access(Access_Status_t Status);
void ECU1_APP_InValid_Acess(Access_Status_t Status);
void ECU1_SPI_IRQ_CallBack(void);
void ECU1_USART_IRQ_CallBack(void);

#endif /* MYAPP_H_ */
