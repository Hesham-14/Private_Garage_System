################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APPLICATION/MyAPP.c 

OBJS += \
./APPLICATION/MyAPP.o 

C_DEPS += \
./APPLICATION/MyAPP.d 


# Each subdirectory must supply rules for building sources it contributes
APPLICATION/MyAPP.o: ../APPLICATION/MyAPP.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU2/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU2/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU2/APPLICATION/MCAL/inc" -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/MyAPP.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

