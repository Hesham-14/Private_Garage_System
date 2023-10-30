/*
 * SPI.c
 *
 * Created: 5/28/2023 6:47:02 PM
 *  Author: hesham mohamed
 */ 

/* =========================================================================
 *                                  Includes
 * =========================================================================
 */

#include "SPI.h"


//===============================================
// GENERIC VARIABLES
//===============================================
SPI_Config_t *Global_SPI_Config;


/*
 * =========================================================================
 * 								Generic Macros
 * =========================================================================
 */

#define SS		4
#define MOSI	5
#define MISO	6
#define SCK		7

/*
 * =========================================================================
 * 								Generic Functions
 * =========================================================================
 */

void MCAL_SPI_DIO_Pins_Init()
{
	if(Global_SPI_Config->SPI_Mode == SPI_MODE_MASTER)
	{
		//1. Enable output pins: PB4/SS, PB5/MOSI, PB7/SCK
		DDRB |= (1<<MOSI)|(1<<SCK)|(1<<SS);
		
		//2. Enable input pin: PB6/MISO
		DDRB &= ~(1<<MISO);
		PORTB |= 1<<MISO;
	}	
	else	//Slave
	{
		//1. Enable output pin: PB5/MISO
		DDRB |= 1<<MISO;
		DDRB &= ~(1<<MOSI) & ~(1<<SS) & ~(1<<SCK);
		PORTB |= (1<<MOSI)|(1<<SCK)|(1<<SS);
	}
}


/*
 * =========================================================================
 * 								APIs Functions Definitions
 * =========================================================================
 */


/**================================================================
 * @Fn			-MCAL_SPI_Init
 * @brief 		-Initializes the SPI 
 * @param [in]	-SPI_Cfg: SPI configurations
 * @retrieval 	-none
 * Note			-none
 */

void MCAL_SPI_Init(SPI_Config_t* SPI_Cfg)
{
	uint8_t Temp_SPCR = 0;
	
	//0. Store SPI Configurations
	Global_SPI_Config = SPI_Cfg;
	
	
	//1. set DIO pins for SPI
	MCAL_SPI_DIO_Pins_Init();
	
	//2. set mode
	Temp_SPCR |= SPI_Cfg->SPI_Mode;
	
	//3. check Double Clock
	SPSR |= SPI_Cfg->CLK_Speed;
	
	//4. set speed for master
	if(SPI_Cfg->SPI_Mode == SPI_MODE_MASTER)
	Temp_SPCR |= SPI_Cfg->Master_Speed;
	
	//5. set data order
	Temp_SPCR |= SPI_Cfg->Data_Order;
	
	//6. set CLK polarity
	Temp_SPCR |= SPI_Cfg->CLK_Polarity;
	
	//7. SPI IRQ
	if(SPI_Cfg->IRQ_Enable == SPI_IRQ_Enable)
	{
		SREG |= (1<<7);
	}
	Temp_SPCR |= SPI_Cfg->IRQ_Enable;
	
	//8. enable SPI
	Temp_SPCR |= 1<<SPE;
	
	//9. Store SPCR value
	SPCR = Temp_SPCR;	
}

void MCAL_SPI_DeInit()
{
	SPCR = 0;
	SPSR &= ~(1<<0);
}

void MCAL_SPI_Send (uint8_t* pTxBuffer,  enum SPI_Polling_Mechanism Polling_En)
{
	//1. Send Data to SPI Data register
	SPDR = *pTxBuffer;
	
	//2. wait until the transmission is done
	if (Polling_En == SPI_Polling_Enable)
	{
		while(!((SPSR>>SPIF) & 1));
	}
}

void MCAL_SPI_MASTER_Send(uint8_t* pTxBuffer)
{
	/* Start Sending Operation */
	//1. Set SS (Low), to listen
	PORTB &= ~(1<<SS);
	
	//2. Send data over SPI
	_delay_ms(10);
	MCAL_SPI_Send(pTxBuffer, SPI_Polling_Enable);
	
	//3. Reset SS to idle mode
}

uint8_t MCAL_SPI_Receive (enum SPI_Polling_Mechanism Polling_En)
{
	//1. wait until the transmission is done
	if (Polling_En == SPI_Polling_Enable)
	{
		while(!((SPSR>>SPIF) & 1));
	}
	
	//2. receive
	return SPDR;
}

/**================================================================
 * @Fn			-MCAL_SPI_Tx_Rx
 * @brief 		-Send & receive from the SPI
 * @param [in]	-data: data that we need to send
 * @retrieval 	-SPDR: the received data
 * Note			-none
 */
uint8_t MCAL_SPI_Tx_Rx(uint8_t* pTxBuffer,  enum SPI_Polling_Mechanism Polling_En)
{
	//1. set data to the SPDR
	SPDR = pTxBuffer;
	
	//2. wait until the transmission is done
	if (Polling_En == SPI_Polling_Enable)
	{
		while(!((SPSR>>SPIF) & 1));
	}
	
	//3. read data from SPDR
	return SPDR;
}


void MCAL_SPI_Send_string(char *p_Data)
{
	uint8_t *terminal = 13;

	while(*p_Data != '\0'){
		MCAL_SPI_Send(p_Data, SPI_Polling_Enable);
		p_Data++;	//send Next Char
	}
	
	/* send Terminal */
	MCAL_SPI_Send(terminal, SPI_Polling_Enable);
}


char* MCAL_SPI_Receive_string()
{
	char data[50], i = 0;
	char* p_data = data;
	char get = 0;
	while(get != 13)
	{
		get = MCAL_SPI_Receive(SPI_Polling_Enable);
		data[i] = get;
		i++;
	}
	data[i-1] = '\0';
	
	return p_data ;
}

//=============================================
//					ISR
//=============================================
void __vector_12 (void) __attribute__((signal));
void __vector_12 (void){
	Global_SPI_Config->p_IRQ_CallBack();

}
