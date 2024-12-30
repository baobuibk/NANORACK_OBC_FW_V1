################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Client_SDO_Receive.c \
../Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Receive.c \
../Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Transmit.c 

OBJS += \
./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Client_SDO_Receive.o \
./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Receive.o \
./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Transmit.o 

C_DEPS += \
./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Client_SDO_Receive.d \
./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Receive.d \
./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Transmit.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/%.o Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/%.su Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/%.cyclo: ../Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/%.c Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DSTM32H745xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/BSP/Systick" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Peripherials/DEFINE" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Peripherials/I2C" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Peripherials" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Peripherials/SPI" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Task/rtc_update" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Peripherials/RTC" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Task/blink_led_debug" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Task/pet_watchdog_m4" -I../Core/Inc -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/BSP/Scheduler" -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-EasyCANopen-2f-CANopen-2f-SDO-2f-SDO_Internal

clean-Core-2f-BSP-2f-EasyCANopen-2f-CANopen-2f-SDO-2f-SDO_Internal:
	-$(RM) ./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Client_SDO_Receive.cyclo ./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Client_SDO_Receive.d ./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Client_SDO_Receive.o ./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Client_SDO_Receive.su ./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Receive.cyclo ./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Receive.d ./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Receive.o ./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Receive.su ./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Transmit.cyclo ./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Transmit.d ./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Transmit.o ./Core/BSP/EasyCANopen/CANopen/SDO/SDO_Internal/CANopen_Server_SDO_Transmit.su

.PHONY: clean-Core-2f-BSP-2f-EasyCANopen-2f-CANopen-2f-SDO-2f-SDO_Internal

