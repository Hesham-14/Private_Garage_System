/*
 * USART.h
 *
 *  Author: hesham mohamed
 */ 

#ifndef USART_H_
#define USART_H_

/*****************************************
-----------     INCLUDES     -------------
*****************************************/
#include "avr/io.h"
#include "avr/interrupt.h"
#include "Macros.h"
#include "Standard_Types.h"

/*****************************************
--------  Configuration Macros  ----------
*****************************************/
#define USART_BAUD				1200
#define USART_FOSC				1000000

/*****************************************
--------------  Variables  ---------------
*****************************************/
typedef enum 
{
	USART_ASYNCH =(0<<6) ,
	USART_SYNC = (1<<6)
}USART_SYNCH_MODE_t;

typedef enum
{
	USART_NORMAL_SPEED,
	USART_DOUBLE_SPEED
}USART_SPEED_MODE_t;

typedef enum 
{
	USART_TRANSMIT_DISABLE,
	USART_TRANSMIT_ENABLE
}USART_TRANSMIT_t;

typedef enum 
{
	USART_RECIEVE_DISABLE,
	USART_RECIEVE_ENABLE
}USART_RECIEVE_t;

typedef enum 
{
	USART_5_Bit_CHAR 	=	(0b00<<1),
	USART_6_Bit_CHAR 	=	(0b01<<1),
	USART_7_Bit_CHAR	 =   (0b10<<1),
	USART_8_Bit_CHAR	 =   (0b011<<1),
	USART_9_Bit_CHAR	 =   (0b111<<1)
}USART_CHAR_SIZE_t;

typedef enum 
{
	USART_NO_PARITY		=(0<<4),
	USART_EVEN_PARITY		=(2<<4),
	USART_ODD_PARITY 	   = (3<<4)
}USART_PARITY_MODE_t;

typedef enum 
{
	USART_1_STOPBIT =	(0<<3),
	USART_2_STOPBIT =(1<<3)
}USART_STOPBIT_t;

typedef enum 
{
	USART_RECIEVER_ENABLE 	=	(1<<7),
	USART_RECIEVER_DISABLE 	=	(0<<7)
}USART_RECIEVE_INT_t;

typedef enum 
{
	USART_TRANSMITER_ENABLE 	=(1<<6),
	USART_TRANSMITER_DISABLE =	(0<<6)
}USART_TRANSMITER_INT_t;


/*****************************************
--------  User type definitions  ---------
*****************************************/
typedef struct
{
	USART_SYNCH_MODE_t		SYNCH_MODE ;
	USART_SPEED_MODE_t		SPEED_MODE;
	USART_TRANSMIT_t		TRANSMIT;
	USART_RECIEVE_t			RECIEVE;
	USART_CHAR_SIZE_t		CHAR_SIZE ;
	USART_PARITY_MODE_t		PARITY_MODE;
	USART_STOPBIT_t			STOPBIT;
	USART_RECIEVE_INT_t		RECIEVE_INT;
	USART_TRANSMITER_INT_t	TRANSMITER_INT;
}USART_Config_t;

/*****************************************
----------  Registers Macros  -----------
*****************************************/
/***********UCSRA***********/
#define RXC 			7
#define TXC 			6
#define UDRE 			5
#define FE 				4
#define DOR				3
#define PE 				2
#define U2X				1
#define MPCM			0

/***********UCSRB***********/
#define RXCIE			7
#define TXCIE			6
#define UDRIE			5
#define RXEN 			4
#define TXEN			3
#define UCSZ2			2
#define RXB8			1
#define TXB8			0

/***********UCSRC***********/
#define URSEL			7
#define UMSEL			6
#define UPM1			5
#define UPM0			4
#define USBS			3
#define UCSZ1			2
#define UCSZ0			1
#define UCPOL			0





/*************************************
---------------  APIs  ---------------
***************************************/
void MCAL_USART_Init(USART_Config_t Config);
void MCAL_USART_Transmit(unsigned short data);
void MCAL_USART_Receive(unsigned short *Ptr_To_Val);
void MCAL_USART_Transmit_string(char *p_Data);
char* MCAL_USART_Receive_string();

void USART_Rec_Int_Enable(void);
void USART_Rec_Int_Disable(void);
void USART_Trans_Int_Enable(void);
void USART_Trans_Int_Disable(void);
void USART_Rec_Enable(void);
void USART_Rec_Disable(void);
void USART_Trans_Enable(void);
void USART_Trans_Disable(void);
void MCAL_USART_callback_Rec_Int(void (*Ptr_to_Func)(void));
void MCAL_USART_callback_Trans_Int(void (*Ptr_to_Func)(void));


#endif
/********************************************************************
 *  END OF FILE:  USART.h
********************************************************************/


