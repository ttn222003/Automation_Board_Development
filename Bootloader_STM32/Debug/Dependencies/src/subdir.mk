################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Dependencies/src/UartBootloaderProtocolDepenedencies.c 

OBJS += \
./Dependencies/src/UartBootloaderProtocolDepenedencies.o 

C_DEPS += \
./Dependencies/src/UartBootloaderProtocolDepenedencies.d 


# Each subdirectory must supply rules for building sources it contributes
Dependencies/src/%.o Dependencies/src/%.su Dependencies/src/%.cyclo: ../Dependencies/src/%.c Dependencies/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ASUS/Documents/Automation_Board_Development/Bootloader_STM32/Cores/include" -I"C:/Users/ASUS/Documents/Automation_Board_Development/Bootloader_STM32/Dependencies/include" -I"C:/Users/ASUS/Documents/Automation_Board_Development/Bootloader_STM32/Platform/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Dependencies-2f-src

clean-Dependencies-2f-src:
	-$(RM) ./Dependencies/src/UartBootloaderProtocolDepenedencies.cyclo ./Dependencies/src/UartBootloaderProtocolDepenedencies.d ./Dependencies/src/UartBootloaderProtocolDepenedencies.o ./Dependencies/src/UartBootloaderProtocolDepenedencies.su

.PHONY: clean-Dependencies-2f-src

