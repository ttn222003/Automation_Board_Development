################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Cores/src/UartBootloaderProtocolCore.c 

OBJS += \
./Cores/src/UartBootloaderProtocolCore.o 

C_DEPS += \
./Cores/src/UartBootloaderProtocolCore.d 


# Each subdirectory must supply rules for building sources it contributes
Cores/src/%.o Cores/src/%.su Cores/src/%.cyclo: ../Cores/src/%.c Cores/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ASUS/Documents/Automation_Board_Development/Bootloader_STM32/Cores/include" -I"C:/Users/ASUS/Documents/Automation_Board_Development/Bootloader_STM32/Dependencies/include" -I"C:/Users/ASUS/Documents/Automation_Board_Development/Bootloader_STM32/Platform/include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Cores-2f-src

clean-Cores-2f-src:
	-$(RM) ./Cores/src/UartBootloaderProtocolCore.cyclo ./Cores/src/UartBootloaderProtocolCore.d ./Cores/src/UartBootloaderProtocolCore.o ./Cores/src/UartBootloaderProtocolCore.su

.PHONY: clean-Cores-2f-src

