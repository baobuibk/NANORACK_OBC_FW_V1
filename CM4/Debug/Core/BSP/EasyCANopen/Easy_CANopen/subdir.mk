################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Other_Node.c \
../Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_This_Node.c \
../Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Thread.c 

OBJS += \
./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Other_Node.o \
./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_This_Node.o \
./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Thread.o 

C_DEPS += \
./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Other_Node.d \
./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_This_Node.d \
./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Thread.d 


# Each subdirectory must supply rules for building sources it contributes
Core/BSP/EasyCANopen/Easy_CANopen/%.o Core/BSP/EasyCANopen/Easy_CANopen/%.su Core/BSP/EasyCANopen/Easy_CANopen/%.cyclo: ../Core/BSP/EasyCANopen/Easy_CANopen/%.c Core/BSP/EasyCANopen/Easy_CANopen/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DSTM32H745xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/BSP/Systick" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Peripherials/DEFINE" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Peripherials/I2C" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Peripherials" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Peripherials/SPI" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Task/rtc_update" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Peripherials/RTC" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Task/blink_led_debug" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Task/pet_watchdog_m4" -I../Core/Inc -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/BSP/Scheduler" -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/MSC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-BSP-2f-EasyCANopen-2f-Easy_CANopen

clean-Core-2f-BSP-2f-EasyCANopen-2f-Easy_CANopen:
	-$(RM) ./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Other_Node.cyclo ./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Other_Node.d ./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Other_Node.o ./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Other_Node.su ./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_This_Node.cyclo ./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_This_Node.d ./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_This_Node.o ./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_This_Node.su ./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Thread.cyclo ./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Thread.d ./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Thread.o ./Core/BSP/EasyCANopen/Easy_CANopen/Easy_CANopen_Thread.su

.PHONY: clean-Core-2f-BSP-2f-EasyCANopen-2f-Easy_CANopen

