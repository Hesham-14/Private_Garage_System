################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APPLICATION/MCAL/STM32F103C6_EXTI_Driver.c \
../APPLICATION/MCAL/STM32F103C6_GPIO_Driver.c \
../APPLICATION/MCAL/STM32F103C6_RCC_Driver.c \
../APPLICATION/MCAL/STM32F103C6_SPI_Driver.c \
../APPLICATION/MCAL/STM32F103C6_TIMER_Driver.c \
../APPLICATION/MCAL/STM32F103C6_USART_Driver.c 

OBJS += \
./APPLICATION/MCAL/STM32F103C6_EXTI_Driver.o \
./APPLICATION/MCAL/STM32F103C6_GPIO_Driver.o \
./APPLICATION/MCAL/STM32F103C6_RCC_Driver.o \
./APPLICATION/MCAL/STM32F103C6_SPI_Driver.o \
./APPLICATION/MCAL/STM32F103C6_TIMER_Driver.o \
./APPLICATION/MCAL/STM32F103C6_USART_Driver.o 

C_DEPS += \
./APPLICATION/MCAL/STM32F103C6_EXTI_Driver.d \
./APPLICATION/MCAL/STM32F103C6_GPIO_Driver.d \
./APPLICATION/MCAL/STM32F103C6_RCC_Driver.d \
./APPLICATION/MCAL/STM32F103C6_SPI_Driver.d \
./APPLICATION/MCAL/STM32F103C6_TIMER_Driver.d \
./APPLICATION/MCAL/STM32F103C6_USART_Driver.d 


# Each subdirectory must supply rules for building sources it contributes
APPLICATION/MCAL/STM32F103C6_EXTI_Driver.o: ../APPLICATION/MCAL/STM32F103C6_EXTI_Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/MCAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/MCAL/STM32F103C6_EXTI_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APPLICATION/MCAL/STM32F103C6_GPIO_Driver.o: ../APPLICATION/MCAL/STM32F103C6_GPIO_Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/MCAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/MCAL/STM32F103C6_GPIO_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APPLICATION/MCAL/STM32F103C6_RCC_Driver.o: ../APPLICATION/MCAL/STM32F103C6_RCC_Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/MCAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/MCAL/STM32F103C6_RCC_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APPLICATION/MCAL/STM32F103C6_SPI_Driver.o: ../APPLICATION/MCAL/STM32F103C6_SPI_Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/MCAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/MCAL/STM32F103C6_SPI_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APPLICATION/MCAL/STM32F103C6_TIMER_Driver.o: ../APPLICATION/MCAL/STM32F103C6_TIMER_Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/MCAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/MCAL/STM32F103C6_TIMER_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APPLICATION/MCAL/STM32F103C6_USART_Driver.o: ../APPLICATION/MCAL/STM32F103C6_USART_Driver.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/MCAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/MCAL/STM32F103C6_USART_Driver.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

