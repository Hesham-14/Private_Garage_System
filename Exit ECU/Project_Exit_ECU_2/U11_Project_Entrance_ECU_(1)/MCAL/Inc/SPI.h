/*
 * SPI.h
 *
 * Created: 5/29/2023 6:06:41 PM
 *  Author: hesham mohamed
 */ 


#ifndef SPI_H_
#define SPI_H_



//===============================================
//Includes
//===============================================
#include <avr/io.h>
#include <avr/iom32.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


//===============================================
//User type definitions (structures), what i need to know from the user
//===============================================
typedef struct
{
	uint8_t SPI_Mode			;		// Specifies the Master/Slave mode
										// This parameter can be a value of @ref SPI_MODE_define
	
	uint8_t Master_Speed		;		// Specifies the speed of master SCK
										// This parameter can be a value of @ref SPI_MASTER_SPEED_define
	
	uint8_t Data_Order			;		// Specifies which bit of data word is transmitted first (LSB/MSB)
										// This parameter can be a value of @ref SPI_DATA_ORDER_define
	
	uint8_t CLK_Polarity		;		// Specifies for what state in SCK sending starts
										// This parameter can be a value of @ref SPI_CLK_Polarity_define
	
	uint8_t CLK_Speed			;		// Specifies SPI Clock speed (high or low)
										// This parameter can be a value of @ref SPI_CLK_SPEED_define
	
	uint8_t IRQ_Enable			;		// enable or disable SPI interrupt request
										// This parameter can be a value of @ref SPI_IRQ_define
	
	void (*p_IRQ_CallBack)(void);		// Pointer to IRQ call back function which will be called once IRQ happens					
}SPI_Config_t;

enum SPI_Polling_Mechanism{
	SPI_Polling_Enable,
	SPI_Polling_Disable
};

//===============================================
// Macros
//===============================================
#define SREG *(volatile uint8_t*)(0x20+0x3F) 
#define SS		4
#define MOSI	5
#define MISO	6
#define SCK		7

#define SPI_DIO		DIOB

//===============================================
// Macros Configuration References
//===============================================

//@ref SPI_MODE_define
#define SPI_MODE_MASTER						(1<<MSTR)
#define SPI_MODE_SLAVE						(0<<MSTR)

//@ref SPI_CLK_SPEED_define
#define SPI_CLK_SPEED_Normal				(uint8_t)(0)
#define SPI_CLK_SPEED_Double				(uint8_t)(1)

//@ref SPI_MASTER_SPEED_define
#define SPI_MASTER_SPEED_f_4				(0<<SPR0)
#define SPI_MASTER_SPEED_f_16				(1<<SPR0)
#define SPI_MASTER_SPEED_f_64				(2<<SPR0)
#define SPI_MASTER_SPEED_f_128				(3<<SPR0)
//SPI2X
#define SPI_MASTER_SPEED_Double_f_2			(0<<SPR0)
#define SPI_MASTER_SPEED_Double_f_8			(1<<SPR0)
#define SPI_MASTER_SPEED_Double_f_32		(2<<SPR0)
#define SPI_MASTER_SPEED_Double_f_64		(3<<SPR0)


//@ref SPI_DATA_ORDER_define
#define SPI_DATA_ORDER_MSB					(0<<DORD)
#define SPI_DATA_ORDER_LSB					(1<<DORD)

//@ref SPI_CLK_Polarity_define
#define SPI_CLK_Polarity_Leading_Rising		(0<<CPOL)|(0<<CPHA)
#define SPI_CLK_Polarity_Leading_Falling	(1<<CPOL)|(1<<CPHA)

//@ref SPI_IRQ_define
#define SPI_IRQ_None						(0<<SPIE)
#define SPI_IRQ_Enable						(1<<SPIE)


//===============================================
// APIs Supported by "MCAL EXTI DRIVER"
//===============================================
void MCAL_SPI_Init(SPI_Config_t* SPI_Cfg);
void MCAL_SPI_DeInit();
void MCAL_SPI_DIO_Pins_Init();

void MCAL_SPI_Send (uint8_t* pTxBuffer,  enum SPI_Polling_Mechanism Polling_En);
void MCAL_SPI_MASTER_Send(uint8_t* pTxBuffer);
uint8_t MCAL_SPI_Receive (enum SPI_Polling_Mechanism Polling_En);


uint8_t MCAL_SPI_Tx_Rx(uint8_t* pTxBuffer,  enum SPI_Polling_Mechanism Polling_En);

void MCAL_SPI_Send_string(char *p_Data);
char* MCAL_SPI_Receive_string();




#endif /* SPI_H_ */