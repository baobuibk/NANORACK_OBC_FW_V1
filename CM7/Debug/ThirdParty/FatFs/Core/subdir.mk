################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/FatFs_Shared_Device/Middlewares/Third_Party/FatFs/src/diskio.c \
D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/FatFs_Shared_Device/Middlewares/Third_Party/FatFs/src/ff.c \
D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/FatFs_Shared_Device/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c 

OBJS += \
./ThirdParty/FatFs/Core/diskio.o \
./ThirdParty/FatFs/Core/ff.o \
./ThirdParty/FatFs/Core/ff_gen_drv.o 

C_DEPS += \
./ThirdParty/FatFs/Core/diskio.d \
./ThirdParty/FatFs/Core/ff.d \
./ThirdParty/FatFs/Core/ff_gen_drv.d 


# Each subdirectory must supply rules for building sources it contributes
ThirdParty/FatFs/Core/diskio.o: D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/FatFs_Shared_Device/Middlewares/Third_Party/FatFs/src/diskio.c ThirdParty/FatFs/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DSTM32H745xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I../Core/Inc -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFs/Core" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFs/Drivers" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFs/Option" -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
ThirdParty/FatFs/Core/ff.o: D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/FatFs_Shared_Device/Middlewares/Third_Party/FatFs/src/ff.c ThirdParty/FatFs/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DSTM32H745xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I../Core/Inc -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFs/Core" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFs/Drivers" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFs/Option" -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"
ThirdParty/FatFs/Core/ff_gen_drv.o: D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/FatFs_Shared_Device/Middlewares/Third_Party/FatFs/src/ff_gen_drv.c ThirdParty/FatFs/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DCORE_CM7 -DSTM32H745xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I../Core/Inc -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFs/Core" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFs/Drivers" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM7/ThirdParty/FatFs/Option" -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-ThirdParty-2f-FatFs-2f-Core

clean-ThirdParty-2f-FatFs-2f-Core:
	-$(RM) ./ThirdParty/FatFs/Core/diskio.cyclo ./ThirdParty/FatFs/Core/diskio.d ./ThirdParty/FatFs/Core/diskio.o ./ThirdParty/FatFs/Core/diskio.su ./ThirdParty/FatFs/Core/ff.cyclo ./ThirdParty/FatFs/Core/ff.d ./ThirdParty/FatFs/Core/ff.o ./ThirdParty/FatFs/Core/ff.su ./ThirdParty/FatFs/Core/ff_gen_drv.cyclo ./ThirdParty/FatFs/Core/ff_gen_drv.d ./ThirdParty/FatFs/Core/ff_gen_drv.o ./ThirdParty/FatFs/Core/ff_gen_drv.su

.PHONY: clean-ThirdParty-2f-FatFs-2f-Core

