@echo off

set UNITY_DIR=../ThirdParty/Unity
set CMOCK_DIR=../ThirdParty/CMock

set GPIO=hal_gpio
set PRODUCT=stm32f1xx
set CONFIG=../Test/ConfigForGenMock/cmock_gpio.yaml

if "%PRODUCT%"=="stm32f1xx" (
    set DRIVER_DIR=STM32F1xx_HAL_Driver
)

echo Generating mock for %PRODUCT%_%GPIO%.h

ruby -I%UNITY_DIR% %CMOCK_DIR%/lib/cmock.rb ^
./../Drivers/%DRIVER_DIR%/Inc/%PRODUCT%_%GPIO%.h ^
-o %CONFIG%