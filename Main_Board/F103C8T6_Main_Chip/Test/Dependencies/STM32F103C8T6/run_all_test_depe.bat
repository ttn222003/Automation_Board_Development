@echo off

set PERIPHERALS=GPIO UART

for %%m in (%PERIPHERALS%) do (
    echo.

    cd %%m

    echo ==============================
    echo Running tests for %%m...
    echo ==============================

    call make -s run

    call make clean

    cd ..
)

echo ==============================
echo Finished running all tests!
echo ==============================