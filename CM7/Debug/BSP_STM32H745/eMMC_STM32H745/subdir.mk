################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../BSP_STM32H745/eMMC_STM32H745/emmc_stm32h745zi.c 

OBJS += \
./BSP_STM32H745/eMMC_STM32H745/emmc_stm32h745zi.o 

C_DEPS += \
./BSP_STM32H745/eMMC_STM32H745/emmc_stm32h745zi.d 


# Each subdirectory must supply rules for building sources it contributes
BSP_STM32H745/eMMC_STM32H745/%.o BSP_STM32H745/eMMC_STM32H745/%.su BSP_STM32H745/eMMC_STM32H745/%.cyclo: ../BSP_STM32H745/eMMC_STM32H745/%.c BSP_STM32H745/eMMC_STM32H745/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DSTM32H745xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I../Core/Inc -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/BSP_STM32H745/eMMC_STM32H745" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFs/Core" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFs/Drivers" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFs/Option" -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-BSP_STM32H745-2f-eMMC_STM32H745

clean-BSP_STM32H745-2f-eMMC_STM32H745:
	-$(RM) ./BSP_STM32H745/eMMC_STM32H745/emmc_stm32h745zi.cyclo ./BSP_STM32H745/eMMC_STM32H745/emmc_stm32h745zi.d ./BSP_STM32H745/eMMC_STM32H745/emmc_stm32h745zi.o ./BSP_STM32H745/eMMC_STM32H745/emmc_stm32h745zi.su

.PHONY: clean-BSP_STM32H745-2f-eMMC_STM32H745

