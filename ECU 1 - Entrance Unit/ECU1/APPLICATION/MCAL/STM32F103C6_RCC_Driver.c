/*
 * STM32F103C6_RCC_Driver.c
 *
 *  Created on: May 6, 2023
 *      Author: hesham mohamed
 */


#include "STM32F103C6_RCC_Driver.h"

/*
 * =========================================================================
 * 								Global Variables
 * =========================================================================
 */

uint32_t APBPrescTable[] = {0, 0, 0, 0, 1, 2, 3, 4};
uint32_t AHBPrescTable[] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8};




/*
 * =========================================================================
 * 								Generic Functions
 * =========================================================================
 */
/*================================================================
 * @Fn			-MCAL_RCC_GetSYS_CLKFreq
 * @brief 		-Get SYS_CLK value
 * @param [in]	-none
 * @retval 		-SYS_CLK value
 * Note			-none
 */
uint32_t MCAL_RCC_GetSYS_CLKFreq(void)
{
	switch(RCC->CFGR >>2 & 0b11)
	{
	case 0:
		return HSI_RC_CLK;
		break;
	case 1:
		return HSE_RC_CLK;
		break;
	case 2:
		return PLL_RC_CLK;
		break;
	default:
		return HSI_RC_CLK;
		break;
	}
}


/*================================================================
 * @Fn			-MCAL_RCC_GetHCLKFreq
 * @brief 		-Get HCLK value
 * @param [in]	-none
 * @retval 		-HCLK value
 * Note			-none
 */
uint32_t MCAL_RCC_GetHCLKFreq(void)
{
	// Get SYS_CLK source Compute HCLK Frequency
	return ( MCAL_RCC_GetSYS_CLKFreq() >> APBPrescTable[(RCC->CFGR >> 4) & 0b1111] );
}


/*================================================================
 * @Fn			-MCAL_RCC_GetPCLK1Freq
 * @brief 		-Get PCLK1 value
 * @param [in]	-none
 * @retval 		-PCLK1 value
 * Note			-none
 */
uint32_t MCAL_RCC_GetPCLK1Freq(void)
{
	// Get HCLK source Compute PCLK1 Frequency
	return ( MCAL_RCC_GetHCLKFreq() >> APBPrescTable[(RCC->CFGR >> 8) & 0b111] );
}


/*================================================================
 * @Fn			-MCAL_RCC_GetPCLK2Freq
 * @brief 		-Get PCLK2 value
 * @param [in]	-none
 * @retval 		-PCLK2 value
 * Note			-none
 */
uint32_t MCAL_RCC_GetPCLK2Freq(void)
{
	// Get HCLK source Compute PCLK2 Frequency
	return ( MCAL_RCC_GetHCLKFreq() >> APBPrescTable[(RCC->CFGR >> 11) & 0b111] );
}




/*
 * =========================================================================
 * 								IRQ Functions
 * =========================================================================
 */
