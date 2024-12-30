################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Peripherials/Ex_WatchDog/ex_watchdog.c 

OBJS += \
./Core/Peripherials/Ex_WatchDog/ex_watchdog.o 

C_DEPS += \
./Core/Peripherials/Ex_WatchDog/ex_watchdog.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Peripherials/Ex_WatchDog/%.o Core/Peripherials/Ex_WatchDog/%.su Core/Peripherials/Ex_WatchDog/%.cyclo: ../Core/Peripherials/Ex_WatchDog/%.c Core/Peripherials/Ex_WatchDog/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DSTM32H745xx -DUSE_FULL_LL_DRIVER -DUSE_HAL_DRIVER -c -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Peripherials/DEFINE" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/BSP/Systick" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Peripherials/SPI" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Task/blink_led_debug" -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/Task/pet_watchdog_m4" -I../Core/Inc -I"D:/LAB_PROJECT_7.7.7.7.7.7.7/stm32cubeide/prototype_obc_stm32h7/CM4/Core/BSP/Scheduler" -I../../Drivers/STM32H7xx_HAL_Driver/Inc -I../../Drivers/CMSIS/Device/ST/STM32H7xx/Include -I../../Drivers/CMSIS/Include -I../../Drivers/STM32H7xx_HAL_Driver/Inc/Legacy -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Peripherials-2f-Ex_WatchDog

clean-Core-2f-Peripherials-2f-Ex_WatchDog:
	-$(RM) ./Core/Peripherials/Ex_WatchDog/ex_watchdog.cyclo ./Core/Peripherials/Ex_WatchDog/ex_watchdog.d ./Core/Peripherials/Ex_WatchDog/ex_watchdog.o ./Core/Peripherials/Ex_WatchDog/ex_watchdog.su

.PHONY: clean-Core-2f-Peripherials-2f-Ex_WatchDog

