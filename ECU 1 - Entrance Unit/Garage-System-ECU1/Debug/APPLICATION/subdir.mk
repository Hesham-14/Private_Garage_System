################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APPLICATION/MyApp.c 

OBJS += \
./APPLICATION/MyApp.o 

C_DEPS += \
./APPLICATION/MyApp.d 


# Each subdirectory must supply rules for building sources it contributes
APPLICATION/MyApp.o: ../APPLICATION/MyApp.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU1/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU1/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU1/APPLICATION/MCAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/MyApp.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

