idf_component_register(SRCS "main.c"
                    REQUIRES esp_driver_uart esp_driver_gpio Protocol ESP32C3
                    INCLUDE_DIRS ".")
