################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include APPLICATION/MCAL/subdir.mk
-include APPLICATION/HAL/subdir.mk
-include APPLICATION/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
U11-Garage-System-UART.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
U11-Garage-System-UART.list \

OBJCOPY_BIN += \
U11-Garage-System-UART.bin \


# All Target
all: U11-Garage-System-UART.elf secondary-outputs

# Tool invocations
U11-Garage-System-UART.elf: $(OBJS) $(USER_OBJS) C:\Users\hesham\ mohamed\STM32CubeIDE\workspace_2_1.4.0\U11-Garage-System-UART\STM32F103C6TX_FLASH.ld
	arm-none-eabi-gcc -gdwarf-2 -o "U11-Garage-System-UART.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"C:\Users\hesham mohamed\STM32CubeIDE\workspace_2_1.4.0\U11-Garage-System-UART\STM32F103C6TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="U11-Garage-System-UART.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

U11-Garage-System-UART.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "U11-Garage-System-UART.list"
	@echo 'Finished building: $@'
	@echo ' '

U11-Garage-System-UART.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "U11-Garage-System-UART.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
