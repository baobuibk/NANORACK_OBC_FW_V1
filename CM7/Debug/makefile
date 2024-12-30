################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include USB_DEVICE/Target/subdir.mk
-include USB_DEVICE/App/subdir.mk
-include ThirdParty/FatFS_R015/Option/subdir.mk
-include ThirdParty/FatFS_R015/Drivers/subdir.mk
-include ThirdParty/FatFS_R015/Core/subdir.mk
-include Middlewares/ST/STM32_USB_Device_Library/subdir.mk
-include Drivers/STM32H7xx_HAL_Driver/subdir.mk
-include Core/Task/rtc_update/subdir.mk
-include Core/Task/dualcore_cmd/subdir.mk
-include Core/Task/LED_Blink/subdir.mk
-include Core/Task/Fake_EXP/subdir.mk
-include Core/Startup/subdir.mk
-include Core/Src/subdir.mk
-include Core/Peripherials/SPI/subdir.mk
-include Core/Peripherials/RTC/subdir.mk
-include Common/Src/subdir.mk
-include BSP/eMMC_STM32H745/subdir.mk
-include BSP/VCP_Ringbuffer/subdir.mk
-include BSP/System_IPC/subdir.mk
-include BSP/SysTick/subdir.mk
-include BSP/Scheduler_M7/subdir.mk
-include BSP/Logger/subdir.mk
-include BSP/FileSystem/subdir.mk
-include BSP/CMDLine_M7/subdir.mk
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


BUILD_ARTIFACT_NAME := prototype_obc_stm32h7_CM7
BUILD_ARTIFACT_EXTENSION := elf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
prototype_obc_stm32h7_CM7.elf \

MAP_FILES += \
prototype_obc_stm32h7_CM7.map \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
prototype_obc_stm32h7_CM7.list \

OBJCOPY_HEX += \
prototype_obc_stm32h7_CM7.hex \

OBJCOPY_BIN += \
prototype_obc_stm32h7_CM7.bin \


# All Target
all: main-build

# Main-build Target
main-build: prototype_obc_stm32h7_CM7.elf secondary-outputs

# Tool invocations
prototype_obc_stm32h7_CM7.elf prototype_obc_stm32h7_CM7.map: $(OBJS) $(USER_OBJS) D:\LAB_PROJECT_7.7.7.7.7.7.7\stm32cubeide\prototype_obc_stm32h7\CM7\STM32H745ZITX_FLASH.ld makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-gcc -o "prototype_obc_stm32h7_CM7.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m7 -T"D:\LAB_PROJECT_7.7.7.7.7.7.7\stm32cubeide\prototype_obc_stm32h7\CM7\STM32H745ZITX_FLASH.ld" --specs=nosys.specs -Wl,-Map="prototype_obc_stm32h7_CM7.map" -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -u _printf_float -u _scanf_float -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

prototype_obc_stm32h7_CM7.list: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "prototype_obc_stm32h7_CM7.list"
	@echo 'Finished building: $@'
	@echo ' '

prototype_obc_stm32h7_CM7.hex: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "prototype_obc_stm32h7_CM7.hex"
	@echo 'Finished building: $@'
	@echo ' '

prototype_obc_stm32h7_CM7.bin: $(EXECUTABLES) makefile objects.list $(OPTIONAL_TOOL_DEPS)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "prototype_obc_stm32h7_CM7.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) default.size.stdout prototype_obc_stm32h7_CM7.bin prototype_obc_stm32h7_CM7.elf prototype_obc_stm32h7_CM7.hex prototype_obc_stm32h7_CM7.list prototype_obc_stm32h7_CM7.map
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX) $(OBJCOPY_BIN)

fail-specified-linker-script-missing:
	@echo 'Error: Cannot find the specified linker script. Check the linker settings in the build configuration.'
	@exit 2

warn-no-linker-script-specified:
	@echo 'Warning: No linker script specified. Check the linker settings in the build configuration.'

.PHONY: all clean dependents main-build fail-specified-linker-script-missing warn-no-linker-script-specified

-include ../makefile.targets
