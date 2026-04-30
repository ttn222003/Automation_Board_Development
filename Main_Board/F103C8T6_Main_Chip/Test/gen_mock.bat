@echo off

set UNITY_DIR=../ThirdParty/Unity
set CMOCK_DIR=../ThirdParty/CMock
set CONFIG_DIR=./ConfigForGenMock

set PERIPHERALS=gpio uart
set PRODUCT=stm32f1xx

if "%PRODUCT%"=="stm32f1xx" (
    set DRIVER_DIR=STM32F1xx_HAL_Driver
)

set HEADER_DIR=../Drivers/%DRIVER_DIR%/Inc

echo ==============================
echo Generating CMock files...
echo ==============================

for %%m in (%PERIPHERALS%) do (
    echo.
    echo [INFO] Generating mock for %%m...

    ruby -I%UNITY_DIR% %CMOCK_DIR%/lib/cmock.rb ^
    %HEADER_DIR%/%PRODUCT%_hal_%%m.h ^
    -o %CONFIG_DIR%/cmock_%%m.yaml

    if errorlevel 1 (
        echo [ERROR] Failed to generate mock for %%m
        exit /b 1
    ) else (
        echo [OK] %%m done
    )
)

echo.
echo ==============================
echo All mocks generated successfully!
echo ==============================