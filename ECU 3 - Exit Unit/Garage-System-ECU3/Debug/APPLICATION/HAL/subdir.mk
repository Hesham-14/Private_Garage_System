################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../APPLICATION/HAL/Buzzer.c \
../APPLICATION/HAL/Gate.c \
../APPLICATION/HAL/Keypad.c \
../APPLICATION/HAL/LCD.c \
../APPLICATION/HAL/Motion.c 

OBJS += \
./APPLICATION/HAL/Buzzer.o \
./APPLICATION/HAL/Gate.o \
./APPLICATION/HAL/Keypad.o \
./APPLICATION/HAL/LCD.o \
./APPLICATION/HAL/Motion.o 

C_DEPS += \
./APPLICATION/HAL/Buzzer.d \
./APPLICATION/HAL/Gate.d \
./APPLICATION/HAL/Keypad.d \
./APPLICATION/HAL/LCD.d \
./APPLICATION/HAL/Motion.d 


# Each subdirectory must supply rules for building sources it contributes
APPLICATION/HAL/Buzzer.o: ../APPLICATION/HAL/Buzzer.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/MCAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/HAL/Buzzer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APPLICATION/HAL/Gate.o: ../APPLICATION/HAL/Gate.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/MCAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/HAL/Gate.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APPLICATION/HAL/Keypad.o: ../APPLICATION/HAL/Keypad.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/MCAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/HAL/Keypad.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APPLICATION/HAL/LCD.o: ../APPLICATION/HAL/LCD.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/MCAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/HAL/LCD.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
APPLICATION/HAL/Motion.o: ../APPLICATION/HAL/Motion.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/HAL/inc" -I"C:/Users/hesham mohamed/STM32CubeIDE/workspace_2_1.4.0/U11-Garage-System-ECU3/APPLICATION/MCAL/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"APPLICATION/HAL/Motion.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

