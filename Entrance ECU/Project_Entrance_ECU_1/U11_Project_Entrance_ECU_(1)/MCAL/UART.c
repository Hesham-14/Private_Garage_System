
/*
 * UART.C
 *
 *  Author: hesham mohamed
 */ 

/*****************************************
-----------     Includes     -------------
*****************************************/
#include "USART.h"


/*****************************************
--------------  Variables  ---------------
*****************************************/
/******* Pointer to functions to be assigned to ISR *******/
static void (*Ptr_To_Trans_Int) (void);
static void (*Ptr_To_Rec_Int) (void);

USART_Config_t global_USART_Config ;



/*****************************************
---------   Generic Functions  -----------
*****************************************/
void USART_Rec_Int_Enable(void)
{
	UCSRB |= 1<< RXCIE ;

}

void USART_Rec_Int_Disable(void)
{
	UCSRB &= ~(1<<RXCIE);
}

void USART_Trans_Int_Enable(void)
{
	UCSRB |= 1<< TXCIE ;
}

void USART_Trans_Int_Disable(void)
{
	UCSRB &= ~ (1<<TXCIE);

}

void USART_Rec_Enable(void)
{
	UCSRB |= 1<< RXEN ;

}

void USART_Rec_Disable(void)
{
	UCSRB &=~(1<<RXEN);

}

void USART_Trans_Enable(void)
{
	UCSRB |= 1<<TXEN;
}

void USART_Trans_Disable(void)
{
	UCSRB &= ~ (1<<TXEN) ;
}




/**************************************
------------  Functions  --------------
***************************************/
void MCAL_USART_Init(USART_Config_t Config) 
{
	unsigned long UBRR_VAL ;
	global_USART_Config = Config ;

	/* 1. Set Baud Rate */
	if ((global_USART_Config.SYNCH_MODE == USART_ASYNCH) && (global_USART_Config.SPEED_MODE == USART_NORMAL_SPEED))
	{
		UBRR_VAL = (unsigned long)((USART_FOSC/(16*USART_BAUD))-1) ;
	}
	else if ((global_USART_Config.SYNCH_MODE == USART_ASYNCH) && (global_USART_Config.SPEED_MODE == USART_DOUBLE_SPEED))
	{
		UBRR_VAL = (unsigned short)((USART_FOSC/(8*USART_BAUD))-1) ;
	}
	else if  ((global_USART_Config.SYNCH_MODE == USART_SYNC))
	{
		UBRR_VAL = (unsigned long)((USART_FOSC/(2*USART_BAUD))-1) ;
	}

	UBRRH =(unsigned char) (UBRR_VAL >> 8);
	UBRRL = (unsigned char) UBRR_VAL;

	/* 2. Enable transmitter */
	if ( global_USART_Config.TRANSMIT == USART_TRANSMIT_ENABLE )
	{
		UCSRB |= (1 << TXEN);

	}
	else if ( global_USART_Config.TRANSMIT == USART_TRANSMIT_DISABLE )
	{

		UCSRB &= ~(1<<TXEN);
	}

	/* 3. Enable receiver */
	if ( global_USART_Config.RECIEVE == USART_RECIEVE_ENABLE )
	{
		UCSRB |= (1 << RXEN);

	}
	else if ( global_USART_Config.RECIEVE == USART_RECIEVE_DISABLE )
	{

		UCSRB &= ~(1<<RXEN);
	}

	/* 3. Set character size */
	if ( global_USART_Config.CHAR_SIZE != USART_9_Bit_CHAR )
	{
		UCSRC = (1 << URSEL) | (global_USART_Config.CHAR_SIZE);

	}
	else if (global_USART_Config.CHAR_SIZE == USART_9_Bit_CHAR )
	{

		UCSRC = (1 << URSEL) |( (global_USART_Config.CHAR_SIZE)&0b11);
		UCSRB |= 1<<2 ;
	}


	/* 4. Set Double speed */
	if ((global_USART_Config.SYNCH_MODE == USART_ASYNCH) && (global_USART_Config.SPEED_MODE == USART_DOUBLE_SPEED))
	{
		UCSRA |= 1<< U2X ;

	}
	/* 5. Set Parity Bit */
	UCSRC |= (1<<URSEL) | global_USART_Config.PARITY_MODE;
	
	/* 6. Set SYNCH ASYNCH */
	UCSRC |= (1<<URSEL) | global_USART_Config.SYNCH_MODE;
	
	/* 7. Set Stop Bit */
	UCSRC |= (1<<URSEL) | global_USART_Config.STOPBIT;
	
	/* 8. RECIEVER INT ENABLE */
	if(global_USART_Config.RECIEVE_INT == USART_RECIEVER_ENABLE)
	UCSRB |= global_USART_Config.RECIEVE_INT;
	
	/* 9. TRANSMITER INT ENABLE */
	if(global_USART_Config.TRANSMITER_INT == USART_TRANSMITER_ENABLE)
	UCSRB |= global_USART_Config.TRANSMITER_INT;
}


void MCAL_USART_Transmit(unsigned short data) {
	/* Wait for empty transmit buffer */
	while (!( UCSRA & (1 << UDRE)));
	
	/* if the data to be transmitted is 9 bits, write the MSB first */
	if ( global_USART_Config.CHAR_SIZE  ==	USART_9_Bit_CHAR )
	{
		UCSRB |= ((data >> 8) & 1);
	}
	/* Put data into buffer, sends the data */
	UDR = (unsigned char)data;
}


void MCAL_USART_Receive(unsigned short *Ptr_To_Val) 
{
	*Ptr_To_Val = 0;
	
	/* Wait for data to be received */
	while (!(UCSRA & (1 << RXC)) );
	
	/* Check if parity error is not ON */
	if(((UCSRA >> PE)& 1) != 1)
	{
		/* if the data to be received is 9 bits, get the MSB first */
		if (global_USART_Config.CHAR_SIZE  ==	USART_9_Bit_CHAR)
		{
			*Ptr_To_Val = (((UCSRB >> 1)& 1) << 8) ;
		}

		/* Get and return received data from buffer */
		*Ptr_To_Val |= UDR;
	}
}

void MCAL_USART_Transmit_string(char *p_Data)
{
	while(*p_Data != '\0'){
		MCAL_USART_Transmit(*p_Data);
		p_Data++;	//send Next Char
	}
}

char* MCAL_USART_Receive_string()
{
	uint8_t data[50], i = 0;
	char* p_data = data;
	char get = 0;
	do{
		MCAL_USART_Receive(&get);
		MCAL_USART_Transmit(get);
		if (get != 13 && get != 10)
		{		
			data[i] = get;
			i++;
		}

	}while(get != 13 && i < 49);
	data[i] = '\0';
	
	return p_data ;
}


/*****************************************
------------  ISR Functions  -------------
*****************************************/
void MCAL_USART_callback_Rec_Int(void (*Ptr_to_Func)(void))
{
	Ptr_To_Rec_Int = Ptr_to_Func;
}
void MCAL_USART_callback_Trans_Int(void (*Ptr_to_Func)(void))
{
	Ptr_To_Trans_Int = Ptr_to_Func;
}

ISR(USART_RXC_vect)
{
	(*Ptr_To_Rec_Int)();
}

ISR(USART_TXC_vect)
{
	(*Ptr_To_Trans_Int)();
}