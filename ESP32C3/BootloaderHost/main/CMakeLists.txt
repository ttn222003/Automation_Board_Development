idf_component_register(
SRCS 
	"BootloaderHost.c"
	"../Cores/src/UartBootloaderProtocolCoresHost.c"
	"../Dependencies/src/UartBootloaderProtocolDepeHost.c"
	"../Platform/ESP32/UartDriver.c"
	"../Platform/ESP32/TimerDriver.c"
	
INCLUDE_DIRS 
	"."
	"../Cores/include"
	"../Dependencies/include"
	"../Platform/include"
)
