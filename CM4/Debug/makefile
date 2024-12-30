################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Drivers/STM32H7xx_HAL_Driver/subdir.mk
-include Core/Task/pet_watchdog_m4/subdir.mk
-include Core/Task/dualcore_cmd/subdir.mk
-include Core/Task/blink_led_debug/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/Peripherials/UART/subdir.mk
-include Core/Peripherials/SPI_ADC/subdir.mk
-include Core/Peripherials/SPI/subdir.mk
-include Core/Peripherials/I2C_Expander/subdir.mk
-include Core/Peripherials/I2C/subdir.mk
-include Core/Peripherials/Ex_WatchDog/subdir.mk
-include Core/BSP/Systick/subdir.mk
-include Core/BSP/System_IPC/subdir.mk
-include Core/BSP/Scheduler/subdir.mk
-include Core/BSP/Logger/subdir.mk
-include Core/BSP/JSMN/subdir.mk
-include Core/BSP/FSP/libfsp/subdir.mk
-include Common/Src/subdir.mk
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

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := prototype_obc_stm32h7_CM4
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
prototype_obc_stm32h7_CM4.elf \

MAP_FILES += \
prototype_obc_stm32h7_CM4.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
prototype_obc_stm32h7_CM4.list \

OBJCOPY_HEX += \
prototype_obc_stm32h7_CM4.hex \

OBJCOPY_BIN += \
prototype_obc_stm32h7_CM4.bin \


# All Target
all: main-build

# Main-build Target
main-build: prototype_obc_stm32h7_CM4.elf secondary-outputs

# Tool invocations
prototype_obc_stm32h7_CM4.elf prototype_obc_stm32h7_CM4.map: $(OBJS) $(USER_OBJS) D:\LAB_PROJECT_7.7.7.7.7.7.7\stm32cubeide\prototype_obc_stm32h7\CM4\STM32H745ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "prototype_obc_stm32h7_CM4.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m4 -T"D:\LAB_PROJECT_7.7.7.7.7.7.7\stm32cubeide\prototype_obc_stm32h7\CM4\STM32H745ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="prototype_obc_stm32h7_CM4.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -u _printf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

prototype_obc_stm32h7_CM4.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "prototype_obc_stm32h7_CM4.list"
	@echo 'Finished building: $@'
	@echo ' '

prototype_obc_stm32h7_CM4.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "prototype_obc_stm32h7_CM4.hex"
	@echo 'Finished building: $@'
	@echo ' '

prototype_obc_stm32h7_CM4.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "prototype_obc_stm32h7_CM4.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout prototype_obc_stm32h7_CM4.bin prototype_obc_stm32h7_CM4.elf prototype_obc_stm32h7_CM4.hex prototype_obc_stm32h7_CM4.list prototype_obc_stm32h7_CM4.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
