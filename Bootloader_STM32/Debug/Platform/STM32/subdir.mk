################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Platform/STM32/TimerDriver.c \
../Platform/STM32/UartDriver.c 

OBJS += \
./Platform/STM32/TimerDriver.o \
./Platform/STM32/UartDriver.o 

C_DEPS += \
./Platform/STM32/TimerDriver.d \
./Platform/STM32/UartDriver.d 


# Each subdirectory must supply rules for building sources it contributes
Platform/STM32/%.o Platform/STM32/%.su Platform/STM32/%.cyclo: ../Platform/STM32/%.c Platform/STM32/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ASUS/Documents/Automation_Board_Development/Bootloader_STM32/Cores/include" -I"C:/Users/ASUS/Documents/Automation_Board_Development/Bootloader_STM32/Dependencies/include" -I"C:/Users/ASUS/Documents/Automation_Board_Development/Bootloader_STM32/Platform/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Platform-2f-STM32

clean-Platform-2f-STM32:
	-$(RM) ./Platform/STM32/TimerDriver.cyclo ./Platform/STM32/TimerDriver.d ./Platform/STM32/TimerDriver.o ./Platform/STM32/TimerDriver.su ./Platform/STM32/UartDriver.cyclo ./Platform/STM32/UartDriver.d ./Platform/STM32/UartDriver.o ./Platform/STM32/UartDriver.su

.PHONY: clean-Platform-2f-STM32

