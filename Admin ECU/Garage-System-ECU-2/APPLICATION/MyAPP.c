/*
 * MyAPP.c
 *
 *  Created on: Oct 3, 2023
 *      Author: hesham mohamed
 */


/* =========================================================================
 *                                  Includes
 * =========================================================================*/
#include "MyAPP.h"




/********* LCD  Macros *********/
#define Welcome_Message			"Welcome To      ETCH Garage ... Ready to Work   Empty Slots: "

/* =========================================================================
 *                                  Typedefs
 * =========================================================================*/
typedef enum
{
	NOT_VALID_ID = '0'	,
	VALID_ID	= '1'
}ID_Check_t ;

typedef enum
{
	DELT_ONE_DRIVER	,
	DELT_ALL_DRIVERS

}DeletingDrivers_t;




/* =========================================================================
 *                             Global Variables
 * =========================================================================*/

UART_Config_t uartCfg;
uint8_t Glob_key_pressed = NULL;
unsigned int Glob_IQ_Flag=0;
uint16_t Glob_UART_char =0;
int i=0;


/********* ADMIN Vars *********/
struct
{
	uint8_t name[NAME_MAX_SIZE+1];
	uint8_t password[NAME_MAX_SIZE+1];

}sGlobAdminArr[NUMBER_OF_ADMINS];
uint8_t admin1[] = ADMIN1_NAME;
uint8_t pasword1[] = ADMIN1_PASSWORD;
uint8_t admin2[] = ADMIN2_NAME;
uint8_t pasword2[] = ADMIN2_PASSWORD;


/********* User Vars *********/
struct
{
	uint8_t name[NAME_MAX_SIZE+1];
	uint8_t ID[NAME_MAX_SIZE+1];
	uint8_t Free_Index;
}sGlobDriverArr[NUMBER_OF_DRIVERS];

uint8_t driver1[] = DRIVER1_NAME;
uint8_t ID1[] = DRIVER1_ID;
uint8_t driver2[] = DRIVER2_NAME;
uint8_t ID2[] = DRIVER2_ID;
uint8_t driver3[] = DRIVER3_NAME;
uint8_t ID3[] = DRIVER3_ID;

uint8_t Glob_Available_Slots = MAX_SLOTS_IN_GARAGE;
//This Array holds the state of Free index of @ref Glob_uint8_tDriverArr to be adding new drivers on it
uint8_t Glob_Driver_Free_Index[NUMBER_OF_DRIVERS] = {1, 1, 1, 1, 1};
uint8_t Glob_NumberOfCurrentUsers = 0;
uint8_t Glob_ArrIndex;

/********* TODO *********/
//TODO session expiring & timer counter

/* =========================================================================
 * 						Generic Functions Prototypes
 * =========================================================================*/
void APP_CLK_init(void);
static void APP_UART_init(void);
static void APP_SPI_init(void);
static void APP_EXTI_init(void);
static void Fill_Admins_Data(void);
static void Fill_Drivers_Data(void);
static uint8_t AdminAccessRequest(void);
void LCD_Start(void);
void LCD_Access_Accepted(uint8_t adminIndex);
void LCD_Access_Denied(void);
void LCD_Option_Menu(void);
void LCD_Delete_Menu(void);
void LCD_Delete_Process(uint8_t status);
uint8_t SystemAdd(void);
uint8_t SystemDelete(void);
uint8_t SystemEdit(void);

uint8_t CompareStrings(uint8_t* S1, uint8_t* S2);
uint8_t DeleteProcess(DeletingDrivers_t DeleteType);
/********* Callback Definition *********/
static void ECU2_UART_IRQ_CallBack(void);
static void ECU2_Entrance_EXT_IRQ_Callback(void);
static void ECU2_Exit_EXT_IRQ_Callback(void);

/* =========================================================================
 * 								Generic Functions
 * =========================================================================*/


/* ==============================================
 * 		Initialization  & Start FCNs
 * ==============================================*/
void APP_CLK_init(void)
{
	//1. GPIO port A clock enable
	RCC_GPIOA_CLK_EN();
	//2. GPIO port B clock enable
	RCC_GPIOB_CLK_EN();
	//3. AFIO clock enable
	RCC_AFIO_CLK_EN();
}

static void APP_EXTI_For_UART_init(void)
{

}

static void APP_UART_init(void)
{

	uartCfg.BaudRate = USART_BAUDRATE_115200;
	uartCfg.HW_Flow_CTRL = HW_FLOW_CTRL_CTS_NONE;
	//	uartCfg.IRQ_EN = USART_IRQ__EN_NONE;
	//	uartCfg.IRQ_CallBack = NULL;
	uartCfg.IRQ_EN = USART_IRQ__EN_RXNEIE;
	uartCfg.IRQ_CallBack = ECU2_UART_IRQ_CallBack;
	uartCfg.Parity = USART_PARITY_NONE;
	uartCfg.PayLoad = USART_PAYLOAD_8B;
	uartCfg.StopBit = UASRT_STOPBIT_1;
	uartCfg.USART_Mode = USART_MODE_Tx_Rx;

	MCAL_UART_Init(USART1, &uartCfg);
	MCAL_UART_GPIO_SetPins(USART1);

	LCD_clear_screen();
	HAL_LCD_Send_String("UART READY");
	MCAL_UART_SendString(USART1, addString("********************* ADMIN DASHBOARD *********************\r\n\0"), '\0');
	delay_ms(200);
}

static void APP_SPI_init(void)
{
	/*------ SPI1 Configurations ------*/
	//PA4 : NSS
	//PA5 : SCK
	//PA6 : MISO
	//PA7 : MOSI

	SPI_Config_t spi1Cfg;

	/* Common Configuration */
	spi1Cfg.Phase = SPI_PHASE_Second_Edge;
	spi1Cfg.Polarity = SPI_IDEAL_High;
	spi1Cfg.Frame_Format = SPI_FRAME_MSB;
	spi1Cfg.Data_Size = SPI_DATA_8_Bit;
	spi1Cfg.Baudrate_Prescaler = SPI_BAUDRATEPRESCALER_4;
	spi1Cfg.Comm_Mode = SPI_COMM_2Lines_FD;

#ifdef ECU2_Act_As_Master
	spi1Cfg.Mode = SPI_MODE_Master;
	spi1Cfg.IRQ_Enable = SPI_IRQ_NONE;
	spi1Cfg.P_ISR_CallBack = NULL;
	spi1Cfg.NSS = SPI_NSS_Software_Slave_RESET;

	/* Initialize SPI Reg & pin */
	MCAL_SPI_Init(SPI1, &spi1Cfg);
	MCAL_SPI_GPIO_SetPins(SPI1);

	/*------ Configure SS Pins ------*/
	GPIO_PinConfig_t pinCfg;

	/* Configure SS1 for ECU1 on PA0 by GPIO */
	pinCfg.GPIO_PinNumber = Entrance_SS;
	pinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
	pinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
	MCAL_GPIO_Init(SS_Port, &pinCfg);
	MCAL_GPIO_WritePin(SS_Port, &pinCfg, GPIO_PIN_SET);

	/* Force the SS to high idle Mode */
	MCAL_GPIO_WritePin(GPIOA, pinCfg.GPIO_PinNumber, GPIO_PIN_SET);

	/* Configure SS2 for ECU3 on PA1 by GPIO */
	pinCfg.GPIO_PinNumber = Exit_SS;
	pinCfg.GPIO_Mode = GPIO_MODE_OUTPUT_PP;
	pinCfg.GPIO_Output_Speed = GPIO_SPEED_10M;
	MCAL_GPIO_Init(SS_Port, &pinCfg);
	MCAL_GPIO_WritePin(SS_Port, &pinCfg, GPIO_PIN_SET);

	/* Force the SS to high idle Mode */
	MCAL_GPIO_WritePin(GPIOA, pinCfg.GPIO_PinNumber, GPIO_PIN_SET);
#endif

#ifdef MCU_Act_As_Slave
	spi1Cfg.Mode = SPI_MODE_Slave;
	spi1Cfg.IRQ_Enable = SPI_IRQ_RXNEIE;
	spi1Cfg.P_ISR_CallBack = Hesham_SPI_IRQ_Callback;
	spi1Cfg.NSS = SPI_NSS_Hardware_Slave;

	/* Initialize SPI Reg & pin */
	MCAL_SPI_Init(SPI1, &spi1Cfg);
	MCAL_SPI_GPIO_SetPins(SPI1);
#endif



	LCD_clear_screen();
	HAL_LCD_Send_String("SPI READY       MODE: Master    ");
	HAL_LCD_Send_String("SS1: PA0        SS2: PA1");
	delay_ms(500);
}

static void APP_EXTI_init(void)
{
	EXTI_PinConfig_t EXTI_cfg;

	/*------ Entrance EXTI Configure ------*/
	EXTI_cfg.EXTI_PIN = Entrance_Pin;
	EXTI_cfg.Trigger_Case = EXTI_TRIGGER_RISING;
	EXTI_cfg.IRQ_EN = EXTI_IRQ_ENABLE;
	EXTI_cfg.P_IRQ_CallBack = ECU2_Entrance_EXT_IRQ_Callback;
	MCAL_EXTI_GPIO_Init(&EXTI_cfg);

	/*------ Exit EXTI Configure ------*/
	EXTI_cfg.EXTI_PIN = Exit_Pin;
	EXTI_cfg.Trigger_Case = EXTI_TRIGGER_RISING;
	EXTI_cfg.IRQ_EN = EXTI_IRQ_ENABLE;
	EXTI_cfg.P_IRQ_CallBack = ECU2_Exit_EXT_IRQ_Callback;
	MCAL_EXTI_GPIO_Init(&EXTI_cfg);

	LCD_clear_screen();
	HAL_LCD_Send_String("Entrance EXTI   Exit EXTI       Both READY");
	delay_ms(500);
}

/* ==============================================
 * 					LCD FCNs
 * ==============================================*/
void LCD_Start(void)
{
	LCD_clear_screen();
	HAL_LCD_Send_String(Welcome_Message);
	HAL_LCD_Send_Number(Glob_Available_Slots);
	delay_ms(1000);

	//print admin
	//	LCD_clear_screen();
	//	for(int i=0; i<2; i++)
	//	{
	//		HAL_LCD_Set_Courser(i+1, 0);
	//		for(int j=0; j<NAME_MAX_SIZE; j++)
	//		{
	//			if(sGlobAdminArr[i].name[j] == '\0')
	//				break;
	//			else
	//				HAL_LCD_Send_Data(sGlobAdminArr[i].name[j]);
	//		}
	//	}
	//	delay_ms(500);
	//

	//	//print added driver which is number 4
	//	LCD_clear_screen();
	//	HAL_LCD_Send_String("Added name:");
	//	HAL_LCD_Send_Command(LCD_BEGIN_AT_SECOND_ROW);
	//	for(int j=0; j<NAME_MAX_SIZE; j++)
	//	{
	//		if(sGlobDriverArr[Glob_ArrIndex].name[j] == '\0')
	//			break;
	//		else
	//		{
	//			HAL_LCD_Set_Courser(3, j);
	//			HAL_LCD_Send_Data(sGlobDriverArr[Glob_ArrIndex].name[j]);
	//		}
	//	}
	//	delay_ms(2000);
	//
	//	HAL_LCD_Send_Command(LCD_DISP_ON);
	//	LCD_clear_screen();
	//	HAL_LCD_Send_String("Added ID:");
	//	HAL_LCD_Set_Courser(2, 0);
	//	for(int j=0; j<NAME_MAX_SIZE; j++)
	//	{
	//		if(sGlobDriverArr[Glob_ArrIndex].ID[j] == '\0')
	//			break;
	//		else
	//		{
	//			HAL_LCD_Set_Courser(3, j);
	//			HAL_LCD_Send_Data(sGlobDriverArr[Glob_ArrIndex].ID[j]);
	//		}
	//	}

	//PrintAllDrivers();
}

void LCD_Access_Accepted(uint8_t adminIndex)
{
	LCD_clear_screen();
	HAL_LCD_Send_String("Hello ");

	/*	print admin name	*/
	for(int j=0; j<NAME_MAX_SIZE; j++)
	{
		if(sGlobAdminArr[adminIndex].name[j] == '\0')
			break;
		else
			HAL_LCD_Send_Data(sGlobAdminArr[adminIndex].name[j]);
	}
	HAL_LCD_Set_Courser(3, 1);
	HAL_LCD_Send_String("Wellcome Back!");
	delay_ms(1000);
	LCD_clear_screen();
}

void LCD_Access_Denied(void)
{
	LCD_clear_screen();
	HAL_LCD_Send_String("Access Denied...");
	delay_ms(750);
	LCD_clear_screen();
}

void LCD_Option_Menu(void)
{
	HAL_LCD_Send_Command(LCD_DISP_ON);
	LCD_clear_screen();
	HAL_LCD_Send_String("ADMIN OPTIONS:");
	HAL_LCD_Send_Command(LCD_BEGIN_AT_SECOND_ROW);
	HAL_LCD_Send_String("  1- Add Driver");
	delay_ms(500);
	HAL_LCD_Send_Command(LCD_BEGIN_AT_THIRD_ROW);
	HAL_LCD_Send_String("  2- Delete Driver");
	delay_ms(500);
	HAL_LCD_Send_Command(LCD_BEGIN_AT_FOURTH_ROW);
	HAL_LCD_Send_String("  3- Edit Driver");

	/*delay_ms(3000);
	HAL_LCD_Send_Command(LCD_DISP_ON);
	LCD_clear_screen();
	HAL_LCD_Send_String("To exit press #");*/
}

void LCD_Delete_Menu(void)
{
	HAL_LCD_Send_Command(LCD_DISP_ON);
	LCD_clear_screen();
	HAL_LCD_Send_String("DELETE OPTIONS:");
	HAL_LCD_Set_Courser(2, 0);
	HAL_LCD_Send_String("1- Delete Driver");
	HAL_LCD_Set_Courser(3, 0);
	HAL_LCD_Send_String("2- Delete All");
	HAL_LCD_Send_Command(LCD_BEGIN_AT_FOURTH_ROW);
	HAL_LCD_Send_String("3- MAIN SCREEN");

	delay_ms(1000);
	HAL_LCD_Send_Command(LCD_DISP_ON);
	LCD_clear_screen();
	HAL_LCD_Send_String("To exit press #");
}

void LCD_Delete_Process(uint8_t status)
{
	HAL_LCD_Send_Command(LCD_DISP_ON);
	LCD_clear_screen();
	if(status == SUCCESSFUL_OPERATION)
	{
		HAL_LCD_Send_String("SUCCESSFUL");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_SECOND_ROW);
		HAL_LCD_Send_String("DELETE");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_THIRD_ROW);
		HAL_LCD_Send_String("PROCESS");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_FOURTH_ROW);
		delay_ms(750);
		LCD_clear_screen();
	}
	else if (status == FAILED_OPERATION)
	{
		HAL_LCD_Send_String("FAAILED");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_SECOND_ROW);
		HAL_LCD_Send_String("DELETE");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_THIRD_ROW);
		HAL_LCD_Send_String("PROCESS");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_FOURTH_ROW);
		delay_ms(750);
		LCD_clear_screen();
	}
}


void LCD_Edit_Process(uint8_t status)
{
	HAL_LCD_Send_Command(LCD_DISP_ON);
	LCD_clear_screen();
	if(status == SUCCESSFUL_OPERATION)
	{
		HAL_LCD_Send_String("SUCCESSFUL");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_SECOND_ROW);
		HAL_LCD_Send_String("EDIT");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_THIRD_ROW);
		HAL_LCD_Send_String("PROCESS");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_FOURTH_ROW);
		delay_ms(750);
		LCD_clear_screen();
	}
	else if (status == FAILED_OPERATION)
	{
		HAL_LCD_Send_String("FAAILED");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_SECOND_ROW);
		HAL_LCD_Send_String("EDIT");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_THIRD_ROW);
		HAL_LCD_Send_String("PROCESS");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_FOURTH_ROW);
		delay_ms(750);
		LCD_clear_screen();
	}
}
/* ==============================================
 * 					SYSTEM FCNs
 * ==============================================*/

static void Fill_Admins_Data(void)
{

	// 1st Admin name
	for(i=0; i<NAME_MAX_SIZE; i++)
	{
		if(admin1[i] != '\0')
			sGlobAdminArr[0].name[i] = admin1[i];
		else
		{
			sGlobAdminArr[0].name[i] = '\0';
			break;
		}
	}
	// 1st Admin password
	for(i=0; i<NAME_MAX_SIZE; i++)
	{
		if(pasword1[i] != '\0')
			sGlobAdminArr[0].password[i] = pasword1[i];
		else
		{
			sGlobAdminArr[0].password[i] = '\0';
			break;
		}
	}

	//	2st Admin name
	for(i=0; i<NAME_MAX_SIZE; i++)
	{
		if(admin2[i] != '\0')
			sGlobAdminArr[1].name[i] = admin2[i];
		else
		{
			sGlobAdminArr[1].name[i] = '\0';
			break;
		}
	}
	//	2st Admin password
	for(i=0; i<NAME_MAX_SIZE; i++)
	{
		if(pasword2[i] != '\0')
			sGlobAdminArr[1].password[i] = pasword2[i];
		else
		{
			sGlobAdminArr[1].password[i] = '\0';
			break;
		}
	}
}

static void Fill_Drivers_Data(void)
{

	// 1st Driver
	for(i=0; i<NAME_MAX_SIZE; i++)
	{
		if(driver1[i] != '\0')
			sGlobDriverArr[0].name[i] = driver1[i];
		else
		{
			sGlobDriverArr[0].name[i] = '\0';
			break;
		}
	}
	for(i=0; i<NAME_MAX_SIZE; i++)
	{
		if(ID1[i] != '\0')
			sGlobDriverArr[0].ID[i] = ID1[i];
		else
		{
			sGlobDriverArr[0].ID[i] = '\0';
			break;
		}
	}
	sGlobDriverArr[0].Free_Index = 0;


	// 2nd Driver
	for(i=0; i<NAME_MAX_SIZE; i++)
	{
		if(driver2[i] != '\0')
			sGlobDriverArr[1].name[i] = driver2[i];
		else
		{
			sGlobDriverArr[1].name[i] = '\0';
			break;
		}
	}
	for(i=0; i<NAME_MAX_SIZE; i++)
	{
		if(ID2[i] != '\0')
			sGlobDriverArr[1].ID[i] = ID2[i];
		else
		{
			sGlobDriverArr[1].ID[i] = '\0';
			break;
		}
	}
	sGlobDriverArr[1].Free_Index = 0;

	// 3rd Driver
	for(i=0; i<NAME_MAX_SIZE; i++)
	{
		if(driver3[i] != '\0')
			sGlobDriverArr[2].name[i] = driver3[i];
		else
		{
			sGlobDriverArr[2].name[i] = '\0';
			break;
		}
	}
	for(i=0; i<NAME_MAX_SIZE; i++)
	{
		if(ID3[i] != '\0')
			sGlobDriverArr[2].ID[i] = ID3[i];
		else
		{
			sGlobDriverArr[2].ID[i] = '\0';
			break;
		}
	}
	sGlobDriverArr[2].Free_Index = 0;

	/*	Store number of users	*/
	Glob_NumberOfCurrentUsers = 3;
}

static uint8_t AdminAccessRequest(void)
{
	/*		Local variables		*/
	uint16_t LOC_RecivedData = 0, LOC_Counter = 0;
	uint8_t LOC_Name[NAME_MAX_SIZE+1] = {0};
	uint8_t LOC_Password[NAME_MAX_SIZE+1] = {0};
	uint8_t LOC_result = 1;
	uint8_t adminIndex = 10;

	/*		Get name		*/
	MCAL_UART_SendString(USART1, addString("NAME: \0"), '\0');

	/* 		LCD Message		*/
	LCD_clear_screen();
	HAL_LCD_Send_String("Welcome to      Dashboard       Please, Enter   Required Data");


	/*		Store name		*/
	do{
		MCAL_UART_ReceiveData(USART1, &LOC_RecivedData, enable);
		MCAL_UART_SendData(USART1, &LOC_RecivedData, enable);
		LOC_Name[LOC_Counter] = (uint8_t) LOC_RecivedData;
		LOC_Counter++;
	}while((LOC_Counter <= NAME_MAX_SIZE) && (LOC_RecivedData != UART_TERMINATE_CHAR));
	LOC_Name[LOC_Counter-1] = '\0';
	LOC_Counter=0;

	/* 		Get Password		*/
	MCAL_UART_SendString(USART1, addString("\r\nPASSWORD: \0"), '\0');
	/*		Store Password		*/
	do{
		MCAL_UART_ReceiveData(USART1, &LOC_RecivedData, enable);
		MCAL_UART_SendData(USART1, &LOC_RecivedData, enable);
		LOC_Password[LOC_Counter] = (uint8_t) LOC_RecivedData;
		LOC_Counter++;
	}while((LOC_Counter <= NAME_MAX_SIZE) && (LOC_RecivedData != UART_TERMINATE_CHAR));
	LOC_Password[LOC_Counter-1] = '\0';
	LOC_Counter=0;

	/*		validate access		*/
	for(LOC_Counter =0; LOC_Counter < NUMBER_OF_ADMINS; LOC_Counter++)
	{
		/*	Search by name	*/
		LOC_result = CompareStrings(LOC_Name, sGlobAdminArr[LOC_Counter].name);
		if(LOC_result == 0)
		{
			adminIndex = LOC_Counter;
			break;
		}
	}

	/*	Check password	if name exists*/
	if(LOC_result == 0 && adminIndex <= NUMBER_OF_ADMINS)
	{
		LOC_result = 1;
		LOC_result = CompareStrings(LOC_Password, sGlobAdminArr[adminIndex].password);
	}
	else
		LOC_result = 1;

	if(LOC_result == 0)
	{
		/*	We has Valid Access	*/
		MCAL_UART_SendString(USART1, addString("\r\n********************* SUCCESSFUL LOGIN *********************\r\n\0"), '\0');
		LCD_Access_Accepted(adminIndex);
		return ADMIN_VALID_ACCESS;
	}
	else
	{
		/*	We has invalid Access	*/
		MCAL_UART_SendString(USART1, addString("\r\n********************* UNSUCCESSFUL LOGIN *********************\r\n\0"), '\0');
		LCD_Access_Denied();
		return ADMIN_INVALID_ACCESS;
	}
}

void SystemStatus()
{
	HAL_LCD_Send_Command(LCD_DISP_ON);
	LCD_clear_screen();
	HAL_LCD_Send_String("PARKING SYSTEM");
	HAL_LCD_Send_Command(LCD_BEGIN_AT_SECOND_ROW);
	HAL_LCD_Send_String("Available Slots:");
	HAL_LCD_Send_Command(LCD_BEGIN_AT_THIRD_ROW);
	HAL_LCD_Send_Data(Glob_Available_Slots);
	delay_ms(1500);
}

uint8_t SystemAdd(void)
{
	if(Glob_NumberOfCurrentUsers < NUMBER_OF_DRIVERS)
	{
		/*	1. Init needed local variables	*/
		uint16_t LOC_ReceivedData = 0;
		uint8_t LOC_ArrIndex, LOC_Counter = 0;

		/*	Find Free array index in Driver Array	*/
		for(LOC_Counter = 0; LOC_Counter < MAX_SLOTS_IN_GARAGE; LOC_Counter++)
		{
			if(sGlobDriverArr[LOC_Counter].Free_Index != 0)
			{
				LOC_ArrIndex = LOC_Counter;
				sGlobDriverArr[LOC_Counter].Free_Index = 0;
				LOC_Counter = 0;
				break;
			}
		}
		LOC_Counter=0;

		/*	UART & LCD Add name message	*/
		MCAL_UART_SendString(USART1, addString("\r\n****************** Adding Driver Data ****************** \r\n\0"), '\0');
		MCAL_UART_SendString(USART1, addString("NAME: \0"), '\0');
		HAL_LCD_Send_Command(LCD_DISP_ON);
		LCD_clear_screen();
		HAL_LCD_Send_String("Driver Username");
		HAL_LCD_Set_Courser(2, 0);
		HAL_LCD_Send_Command(LCD_DISP_ON_CURSOR_BLINK);

		/*	Store Name	*/
		do{
			MCAL_UART_ReceiveData(USART1, &LOC_ReceivedData, enable);
			MCAL_UART_SendData(USART1, &LOC_ReceivedData, enable);
			sGlobDriverArr[LOC_ArrIndex].name[LOC_Counter] = (uint8_t) LOC_ReceivedData;
			LOC_Counter++;
		}while((LOC_Counter <= NAME_MAX_SIZE) && (LOC_ReceivedData != UART_TERMINATE_CHAR));
		sGlobDriverArr[LOC_ArrIndex].ID[LOC_Counter] = '\0';
		LOC_Counter=0;
		LOC_ReceivedData = 0;

		/*	UART & LCD Add ID message	*/
		MCAL_UART_SendString(USART1, addString("\r\nID: \0"), '\0');
		HAL_LCD_Send_Command(LCD_DISP_ON);
		HAL_LCD_Set_Courser(3, 0);
		HAL_LCD_Send_String("Driver ID");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_FOURTH_ROW);
		HAL_LCD_Send_Command(LCD_DISP_ON_CURSOR_BLINK);

		/*	Store ID	*/
		do{
			MCAL_UART_ReceiveData(USART1, &LOC_ReceivedData, enable);
			MCAL_UART_SendData(USART1, &LOC_ReceivedData, enable);
			sGlobDriverArr[LOC_ArrIndex].ID[LOC_Counter] = (uint8_t) LOC_ReceivedData;
			LOC_Counter++;
		}while((LOC_Counter <= NAME_MAX_SIZE) && (LOC_ReceivedData != UART_TERMINATE_CHAR));
		sGlobDriverArr[LOC_ArrIndex].ID[LOC_Counter] = '\0';
		LOC_Counter=0;
		Glob_ArrIndex = LOC_ArrIndex;

		Glob_NumberOfCurrentUsers++;
		MCAL_UART_SendString(USART1, addString("\r\n****************** SUCCESSFUL ADD ****************** \r\n\0"), '\0');
		return SUCCESSFUL_OPERATION;
	}
	else
	{
		/*	FULL ARRAY	*/
		HAL_LCD_Send_Command(LCD_DISP_ON);
		LCD_clear_screen();
		HAL_LCD_Send_String("PARKING GARAGE");
		HAL_LCD_Set_Courser(2, 0);
		HAL_LCD_Send_String("FULL CAPACITY");
		delay_ms(350);
		LCD_clear_screen();
		//Come back to menu loop
		//todo ECU2_Dashboard_APP();

		return FAILED_OPERATION;
	}
}

uint8_t SystemDelete(void)
{
	Glob_key_pressed = NULL;
	LCD_Delete_Menu();
	while(!((Glob_key_pressed>= '1' && Glob_key_pressed <= '2' ) || (Glob_key_pressed == (uint8_t)UART_TERMINATE_CHAR )))
	{
		MCAL_UART_ReceiveData(USART1, &Glob_key_pressed, enable);
		MCAL_UART_SendData(USART1, &Glob_key_pressed, enable);
	}
	if(Glob_key_pressed == OPTION_DELETE_USER)
		return DeleteProcess(DELT_ONE_DRIVER);
	else if (Glob_key_pressed == OPTION_DELETE_USER)
		return DeleteProcess(DELT_ALL_DRIVERS);
	else
		return SystemDelete();
}

uint8_t SystemEdit(void)
{
	/*	 variables	*/
	uint8_t LOC_Counter = 0;
	uint8_t LOC_Username[NAME_MAX_SIZE+1];
	uint8_t LOC_ID[ID_SIZE+1];
	uint8_t LOC_ReceivedData = 0;
	uint8_t LOC_DriverIndex = 0;
	uint8_t LOC_Result = 0;

	/* UART & LCD Display name message	*/
	MCAL_UART_SendString(USART1, addString("\r\n****************** Edit Driver ****************** \r\n\0"), '\0');
	MCAL_UART_SendString(USART1, addString("NAME: \0"), '\0');
	HAL_LCD_Send_Command(LCD_DISP_ON);
	LCD_clear_screen();
	HAL_LCD_Send_String("EDIT DRIVER");
	delay_ms(750);
	LCD_clear_screen();
	HAL_LCD_Send_String("Driver Name:");
	HAL_LCD_Send_Command(LCD_BEGIN_AT_SECOND_ROW);
	HAL_LCD_Send_Command(LCD_DISP_ON_CURSOR_BLINK);

	/*	get name */
	do
	{
		MCAL_UART_ReceiveData(USART1, &LOC_ReceivedData, enable);
		MCAL_UART_SendData(USART1, &LOC_ReceivedData, enable);
		HAL_LCD_Send_Data(LOC_ReceivedData);
		LOC_Username[LOC_Counter] = (uint8_t) LOC_ReceivedData;
		LOC_Counter++;
	}while((LOC_Counter <= NAME_MAX_SIZE) && (LOC_ReceivedData != UART_TERMINATE_CHAR));
	LOC_Username[LOC_Counter] = '\0';
	LOC_Counter = 0;
	LOC_ReceivedData = 0;

	/*	searching	*/
	for(LOC_Counter = 0; LOC_Counter < Glob_NumberOfCurrentUsers; LOC_Counter++)
	{
		/*	Name Search	*/
		LOC_Result = CompareStrings(LOC_Username, sGlobDriverArr[LOC_Counter].name);
		if(LOC_Result == 0)
		{
			LOC_DriverIndex = LOC_Counter;
			break;
		}
		else
			LOC_Result = 1;
	}
	if(LOC_Result == 1)
	{
		MCAL_UART_SendString(USART1, addString("\r\n****************** Driver Not Found ****************** \r\n\0"), '\0');
		LCD_Edit_Process(FAILED_OPERATION);
		return FAILED_OPERATION;
	}

	/* UART & LCD Display ID message	*/
	MCAL_UART_SendString(USART1, addString("\r\nDriver ID: \0"), '\0');
	HAL_LCD_Send_Command(LCD_BEGIN_AT_THIRD_ROW);
	HAL_LCD_Send_String("Driver ID:");
	HAL_LCD_Send_Command(LCD_BEGIN_AT_FOURTH_ROW);

	/*	get ID	*/
	do
	{
		MCAL_UART_ReceiveData(USART1, &LOC_ReceivedData, enable);
		MCAL_UART_SendData(USART1, &LOC_ReceivedData, enable);
		HAL_LCD_Send_Data(LOC_ReceivedData);
		LOC_ID[LOC_Counter] = (uint8_t) LOC_ReceivedData;
		LOC_Counter++;
	}while((LOC_Counter <= NAME_MAX_SIZE) && (LOC_ReceivedData != UART_TERMINATE_CHAR));
	LOC_ID[LOC_Counter] = '\0';
	LOC_Counter = 0;
	LOC_ReceivedData = 0;


	/*	Check ID	*/
	if(LOC_Result == 0 && LOC_DriverIndex <= Glob_NumberOfCurrentUsers && sGlobDriverArr[LOC_DriverIndex].Free_Index == 0)
	{
		LOC_Result = 1;
		LOC_Result = CompareStrings(LOC_ID, &sGlobDriverArr[LOC_DriverIndex].ID);
	}
	else
	{
		LOC_Result = 1;
	}

	/*	Valid Name & ID	*/
	if(LOC_Result == 0)
	{

		MCAL_UART_SendString(USART1, addString("\r\n****************** Driver Found ****************** \r\n\0"), '\0');
		/* update name	*/
		MCAL_UART_SendString(USART1, addString("UPDATE NAME: \0"), '\0');
		HAL_LCD_Send_Command(LCD_DISP_ON);
		LCD_clear_screen();
		HAL_LCD_Send_String("Driver Username");
		HAL_LCD_Set_Courser(2, 0);
		HAL_LCD_Send_Command(LCD_DISP_ON_CURSOR_BLINK);

		/*	Store Name	*/
		do{
			MCAL_UART_ReceiveData(USART1, &LOC_ReceivedData, enable);
			MCAL_UART_SendData(USART1, &LOC_ReceivedData, enable);
			sGlobDriverArr[LOC_DriverIndex].name[LOC_Counter] = (uint8_t) LOC_ReceivedData;
			LOC_Counter++;
		}while((LOC_Counter <= NAME_MAX_SIZE) && (LOC_ReceivedData != UART_TERMINATE_CHAR));
		sGlobDriverArr[LOC_DriverIndex].ID[LOC_Counter] = '\0';
		LOC_Counter=0;
		LOC_ReceivedData = 0;

		/*	UART & LCD Add ID message	*/
		MCAL_UART_SendString(USART1, addString("\r\nUPDATE ID: \0"), '\0');
		HAL_LCD_Send_Command(LCD_BEGIN_AT_THIRD_ROW);
		HAL_LCD_Send_String("Driver ID");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_FOURTH_ROW);
		HAL_LCD_Send_Command(LCD_DISP_ON_CURSOR_BLINK);

		/*	Store ID	*/
		do{
			MCAL_UART_ReceiveData(USART1, &LOC_ReceivedData, enable);
			MCAL_UART_SendData(USART1, &LOC_ReceivedData, enable);
			sGlobDriverArr[LOC_DriverIndex].ID[LOC_Counter] = (uint8_t) LOC_ReceivedData;
			LOC_Counter++;
		}while((LOC_Counter <= NAME_MAX_SIZE) && (LOC_ReceivedData != UART_TERMINATE_CHAR));
		sGlobDriverArr[LOC_DriverIndex].ID[LOC_Counter] = '\0';
		LOC_Counter=0;

		/*	Successful Edit	*/
		MCAL_UART_SendString(USART1, addString("\r\n****************** SUCCESSFUL EDIT ****************** \r\n\0"), '\0');
		LCD_Edit_Process(SUCCESSFUL_OPERATION);
		return SUCCESSFUL_OPERATION;
	}
	else
	{
		MCAL_UART_SendString(USART1, addString("\r\n****************** UNSUCCESSFUL EDIT ****************** \r\n\0"), '\0');
		LCD_Edit_Process(FAILED_OPERATION);
		return FAILED_OPERATION;
	}
}



/* ==============================================
 * 					GENERIC FCNs
 * ==============================================*/
uint8_t DeleteProcess(DeletingDrivers_t DeleteType)
{
	/*	check if we got users	*/
	if(Glob_NumberOfCurrentUsers != 0)
	{
		uint8_t LOC_Counter = 0;
		if(DeleteType == DELT_ONE_DRIVER)
		{
			/*	 variables	*/
			uint8_t LOC_Username[NAME_MAX_SIZE+1];
			uint8_t LOC_ID[ID_SIZE+1];
			uint8_t LOC_ReceivedData = 0;
			uint8_t LOC_DriverIndex = 0;
			uint8_t LOC_Result = 0;

			/* UART & LCD Display name message	*/
			MCAL_UART_SendString(USART1, addString("\r\n****************** Delete Driver ****************** \r\n\0"), '\0');
			MCAL_UART_SendString(USART1, addString("NAME: \0"), '\0');
			HAL_LCD_Send_Command(LCD_DISP_ON);
			LCD_clear_screen();
			HAL_LCD_Send_String("Driver Name:");
			HAL_LCD_Send_Command(LCD_BEGIN_AT_SECOND_ROW);
			HAL_LCD_Send_Command(LCD_DISP_ON_CURSOR_BLINK);

			/*	get name */
			do
			{
				MCAL_UART_ReceiveData(USART1, &LOC_ReceivedData, enable);
				MCAL_UART_SendData(USART1, &LOC_ReceivedData, enable);
				HAL_LCD_Send_Data(LOC_ReceivedData);
				LOC_Username[LOC_Counter] = (uint8_t) LOC_ReceivedData;
				LOC_Counter++;
			}while((LOC_Counter <= NAME_MAX_SIZE) && (LOC_ReceivedData != UART_TERMINATE_CHAR));
			LOC_Username[LOC_Counter] = '\0';
			LOC_Counter = 0;
			LOC_ReceivedData = 0;

			/*	searching	*/
			for(LOC_Counter = 0; LOC_Counter < Glob_NumberOfCurrentUsers; LOC_Counter++)
			{
				/*	Name Search	*/
				LOC_Result = CompareStrings(LOC_Username, sGlobDriverArr[LOC_Counter].name);
				if(LOC_Result == 0)
				{
					LOC_DriverIndex = LOC_Counter;
					break;
				}
				else
					LOC_Result = 1;
			}
			if(LOC_Result == 1)
			{
				MCAL_UART_SendString(USART1, addString("\r\n****************** UNSUCCESSFUL DELETE ****************** \r\n\0"), '\0');
				LCD_Delete_Process(FAILED_OPERATION);
				return FAILED_OPERATION;
			}

			/* UART & LCD Display ID message	*/
			MCAL_UART_SendString(USART1, addString("\r\nDriver ID: \0"), '\0');
			HAL_LCD_Send_Command(LCD_BEGIN_AT_THIRD_ROW);
			HAL_LCD_Send_String("Driver ID:");
			HAL_LCD_Send_Command(LCD_BEGIN_AT_FOURTH_ROW);

			/*	get ID	*/
			do
			{
				MCAL_UART_ReceiveData(USART1, &LOC_ReceivedData, enable);
				MCAL_UART_SendData(USART1, &LOC_ReceivedData, enable);
				HAL_LCD_Send_Data(LOC_ReceivedData);
				LOC_ID[LOC_Counter] = (uint8_t) LOC_ReceivedData;
				LOC_Counter++;
			}while((LOC_Counter <= NAME_MAX_SIZE) && (LOC_ReceivedData != UART_TERMINATE_CHAR));
			LOC_ID[LOC_Counter] = '\0';
			LOC_Counter = 0;
			LOC_ReceivedData = 0;


			/*	Check ID	*/
			if(LOC_Result == 0 && LOC_DriverIndex <= Glob_NumberOfCurrentUsers && sGlobDriverArr[LOC_DriverIndex].Free_Index == 0)
			{
				LOC_Result = 1;
				LOC_Result = CompareStrings(LOC_ID, &sGlobDriverArr[LOC_DriverIndex].ID);
			}
			else
			{
				LOC_Result = 1;
			}

			/*	Valid Name & ID	*/
			if(LOC_Result == 0)
			{
				sGlobDriverArr[LOC_DriverIndex].Free_Index = 1;
				Glob_NumberOfCurrentUsers--;
				MCAL_UART_SendString(USART1, addString("\r\n****************** SUCCESSFUL DELETE ****************** \r\n\0"), '\0');
				LCD_Delete_Process(SUCCESSFUL_OPERATION);
				return SUCCESSFUL_OPERATION;
			}
			else
			{
				MCAL_UART_SendString(USART1, addString("\r\n****************** UNSUCCESSFUL DELETE ****************** \r\n\0"), '\0');
				LCD_Delete_Process(FAILED_OPERATION);
				return FAILED_OPERATION;
			}
		}
		/*	delete all users	*/
		else if (DeleteType == DELT_ALL_DRIVERS)
		{
			for(LOC_Counter = 0; LOC_Counter < NUMBER_OF_DRIVERS; LOC_Counter++)
			{
				sGlobDriverArr[LOC_Counter].Free_Index = 1;
			}
			Glob_NumberOfCurrentUsers = 0;
			MCAL_UART_SendString(USART1, addString("\r\n****************** SUCCESSFUL DELETE ****************** \r\n\0"), '\0');
			LCD_Delete_Process(SUCCESSFUL_OPERATION);
			return SUCCESSFUL_OPERATION;
		}
	}
}

void PrintAllDrivers()
{
	uint8_t LOC_Counter = 0;
	uint8_t LOC_DriverIndex = 0;
	//print added driver which is number 4
	HAL_LCD_Send_Command(LCD_DISP_ON);
	LCD_clear_screen();
	while(LOC_Counter < Glob_NumberOfCurrentUsers && LOC_DriverIndex < NUMBER_OF_DRIVERS)
	{
		LCD_clear_screen();
		HAL_LCD_Send_String("PRINTING USERS:");
		HAL_LCD_Send_Command(LCD_BEGIN_AT_SECOND_ROW);

		if(sGlobDriverArr[LOC_DriverIndex].Free_Index == 0)
		{
			/*	print name	*/
			HAL_LCD_Send_String("NAME: ");
			for(int j=0; j<NAME_MAX_SIZE; j++)
			{
				if(sGlobDriverArr[LOC_DriverIndex].name[j] == '\0')
					break;
				else
				{
					HAL_LCD_Send_Data(sGlobDriverArr[LOC_DriverIndex].name[j]);
				}
			}
			delay_ms(1000);

			/*	print ID	*/

			HAL_LCD_Send_Command(LCD_BEGIN_AT_THIRD_ROW);
			HAL_LCD_Send_String("ID:  ");
			for(int j=0; j<NAME_MAX_SIZE; j++)
			{
				if(sGlobDriverArr[LOC_DriverIndex].ID[j] == '\0')
					break;
				else
				{
					HAL_LCD_Send_Data(sGlobDriverArr[LOC_DriverIndex].ID[j]);
				}
			}
			delay_ms(1000);

			/*	incremend Counter	*/
			LOC_Counter++;
		}

		LOC_DriverIndex++;
	}
}

uint8_t CompareStrings(uint8_t* S1, uint8_t* S2)
{
	uint8_t LOC_Counter = 0;
	uint8_t LOC_Flag =0;
	while(S1[LOC_Counter] != '\0' && S2[LOC_Counter] != '\0')
	{
		if(S1[LOC_Counter] != S2[LOC_Counter])
		{
			LOC_Flag =1;
			break;
		}
		LOC_Counter++;
	}
	return LOC_Flag;
}


/* =========================================================================
 * 							APIs Functions Definitions
 * =========================================================================*/
void ECU2_ADMIN_Dashboard(void)
{
	uint8_t temp;

	/* Options menu	*/
	LCD_Option_Menu();

	//todo get number from admin using keypad
	/*do
	{
		Glob_key_pressed = HAL_keypad_get_key();
	}while((Glob_key_pressed != NULL) || (Glob_key_pressed == (uint8_t)KEYPAD_BACK_SYMBOL ));*/

	while((Glob_key_pressed != UART_TERMINATE_CHAR))
	{
		MCAL_UART_ReceiveData(USART1, &Glob_key_pressed, enable);
		MCAL_UART_SendData(USART1, &Glob_key_pressed, enable);
		if(Glob_key_pressed == OPTION_ADD)
		{
			temp = SystemAdd();
			break;
		}
		else if (Glob_key_pressed == OPTION_DELETE)
		{
			temp = SystemDelete();
		}
		else if (Glob_key_pressed == OPTION_EDIT)
		{
			temp = SystemEdit();
		}
	}


	/*switch(Glob_key_pressed)
	{
	case OPTION_ADD_USER:
		x = SystemAdd();
		break;
	case OPTION_DELETE_USER:
		SystemDelete();
		break;
	case OPTION_EDIT_USER:
		EditUser();
		break;
	default:
		ECU2_Dashboard_APP();
	}*/
	HAL_LCD_Send_Data('(');
	HAL_LCD_Send_Data(temp);
	HAL_LCD_Send_Data(')');
	delay_ms(1000);
}

/*************************************************************************
 * @Fn			- ECU2_Dashboard_APP_SETUP
 * @brief 		- This function is used to make MCAL & HAL initialization.
 * @param [in]	- none
 * @retval 		- none
 **************************************************************************/
void ECU2_Dashboard_APP_SETUP(void)
{
	APP_CLK_init();

	//HAL initialization
	HAL_LCD_Init();
	HAL_keypad_init();

	//MCAL initialization
	APP_UART_init();
	APP_SPI_init();
	APP_EXTI_init();

	//Get data ready
	Fill_Admins_Data();
	Fill_Drivers_Data();

	MCAL_UART_SendString(USART1, addString("********* Ready to test *********\r\n\0"), '\0');
}


void ECU2_Dashboard_APP_RUN(void)
{
	//	HAL_LCD_Send_Data('H');
	//	delay_ms(100);
	//	HAL_LCD_Send_String(" Hey");
	//	HAL_LCD_Send_String(" Man");
	//	delay_ms(500);
	//	LCD_clear_screen();
	//	HAL_LCD_Send_String("Keypad is Ready");
	//	LCD_clear_screen();
	while(1)
	{
		//LCD Welcome window
		LCD_Start();
	}
}


/* =========================================================================
 * 								Callback Functions
 * =========================================================================*/
static void ECU2_UART_IRQ_CallBack(void)
{
	uint16_t LOC_RecivedData = 0;
	MCAL_UART_ReceiveData(USART1, &LOC_RecivedData, disable);
	if(LOC_RecivedData == UART_TERMINATE_CHAR)
	{

		if(AdminAccessRequest() == ADMIN_VALID_ACCESS)
		{
			ECU2_ADMIN_Dashboard();
		}

		delay_ms(500);
		MCAL_UART_ReceiveData(USART1, &LOC_RecivedData, enable);
		MCAL_UART_SendString(USART1, addString("********* Sending to SPI *********\r\n\0"), '\0');
		MCAL_UART_SendData(USART1, &LOC_RecivedData, enable);

		/* Send data to SPI */
		//1. Set SS Low
		MCAL_GPIO_WritePin(SS_Port, Entrance_SS, GPIO_PIN_RESET);
		//2. Send data and receive through SPI
		MCAL_SPI_TxRx(SPI1, &LOC_RecivedData, PollingEnable);
		//2. Reset SS to idle (High)
		MCAL_GPIO_WritePin(SS_Port, Entrance_SS, GPIO_PIN_SET);

		MCAL_UART_SendString(USART1, addString("\r\n********************* ADMIN DASHBOARD *********************\r\n\0"), '\0');
		HAL_LCD_Send_Command(LCD_DISP_ON);
		LCD_clear_screen();
		HAL_LCD_Send_String("Thanks");
		delay_ms(500);
	}
}


static void ECU2_Entrance_EXT_IRQ_Callback(void)
{
	/*		Local variables		*/
	uint8_t		LOC_TxData = SPI_MASTER_ACK;
	uint16_t 	LOC_RxData = 'a';
	uint8_t 	LOC_Counter = 0;
	uint8_t		LOC_Name[NAME_MAX_SIZE+1] = {0};
	uint8_t 	LOC_ID[NAME_MAX_SIZE+1] = {0};
	uint8_t 	LOC_result = 1;
	uint8_t 	DriverIndex = 0;

	//1. Enable Slave: Set SS Low
	MCAL_GPIO_WritePin(SS_Port, Entrance_SS, GPIO_PIN_RESET);

	MCAL_UART_SendString(USART1, addString("********* ENTRANCE Request ID: \0"), '\0');
	MCAL_SPI_TxRx(SPI1, &LOC_RxData, PollingEnable);

	//2. Read data
	//MCAL_SPI_ReceiveData(SPI1, &LOC_RxData, PollingEnable);

	//3. Inform ADMIN
	MCAL_UART_SendData(USART1, &LOC_RxData, enable);
	MCAL_UART_SendString(USART1, addString(" *********\r\n\0"), '\0');

	LCD_clear_screen();
	HAL_LCD_Send_String("----ENTRANCE----");
	HAL_LCD_Send_String("----Request-----");
	HAL_LCD_Send_String("ID: ");
	HAL_LCD_Send_Data(LOC_RxData);

	MCAL_GPIO_WritePin(SS_Port, Entrance_SS, GPIO_PIN_SET);
	delay_ms(1000);
}


static void ECU2_Exit_EXT_IRQ_Callback(void)
{

}
