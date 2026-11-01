idf_component_register(SRCS "main.c"
                    REQUIRES esp_driver_uart esp_driver_gpio Protocol
                    INCLUDE_DIRS ".")
