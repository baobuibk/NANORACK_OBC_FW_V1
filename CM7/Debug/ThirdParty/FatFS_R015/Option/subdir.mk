################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ThirdParty/FatFS_R015/Option/ccsbcs.c \
../ThirdParty/FatFS_R015/Option/syscall.c \
../ThirdParty/FatFS_R015/Option/unicode.c 

OBJS += \
./ThirdParty/FatFS_R015/Option/ccsbcs.o \
./ThirdParty/FatFS_R015/Option/syscall.o \
./ThirdParty/FatFS_R015/Option/unicode.o 

C_DEPS += \
./ThirdParty/FatFS_R015/Option/ccsbcs.d \
./ThirdParty/FatFS_R015/Option/syscall.d \
./ThirdParty/FatFS_R015/Option/unicode.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/FatFS_R015/Option/%.o ThirdParty/FatFS_R015/Option/%.su ThirdParty/FatFS_R015/Option/%.cyclo: ../ThirdParty/FatFS_R015/Option/%.c ThirdParty/FatFS_R015/Option/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DSTM32H745xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/BSP/Scheduler_M7" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/BSP/FileSystem" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/Core/Peripherials/RTC" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/Core/Peripherials/SPI" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/BSP/eMMC_STM32H745" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/BSP/Logger" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/BSP/System_IPC" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFS_R015/Core" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFS_R015/Option" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFS_R015/Drivers" -I../Core/Inc -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ThirdParty-2f-FatFS_R015-2f-Option

clean-ThirdParty-2f-FatFS_R015-2f-Option:
	-$(RM) ./ThirdParty/FatFS_R015/Option/ccsbcs.cyclo ./ThirdParty/FatFS_R015/Option/ccsbcs.d ./ThirdParty/FatFS_R015/Option/ccsbcs.o ./ThirdParty/FatFS_R015/Option/ccsbcs.su ./ThirdParty/FatFS_R015/Option/syscall.cyclo ./ThirdParty/FatFS_R015/Option/syscall.d ./ThirdParty/FatFS_R015/Option/syscall.o ./ThirdParty/FatFS_R015/Option/syscall.su ./ThirdParty/FatFS_R015/Option/unicode.cyclo ./ThirdParty/FatFS_R015/Option/unicode.d ./ThirdParty/FatFS_R015/Option/unicode.o ./ThirdParty/FatFS_R015/Option/unicode.su

.PHONY: clean-ThirdParty-2f-FatFS_R015-2f-Option

