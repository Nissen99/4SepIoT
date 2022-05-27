#include "lorawanDownlinkHandler.h"
#include "terrarium.h"
#include <lora_driver.h>

MessageBufferHandle_t downLinkMessageBuffer;
lora_driver_payload_t* downlinkPayload;

/************************************************************************/
/* loraDownlinkRun
Lytter på down link, og kalder feedAnimalTerrarium hvis der modtages 
data på port 1															*/
/************************************************************************/
inline void loraDownlinkRun()
{
	// this code must be in the loop of a FreeRTOS task!
	xMessageBufferReceive(downLinkMessageBuffer, downlinkPayload, sizeof(lora_driver_payload_t), portMAX_DELAY);
	
	printf("DOWN LINK: from port: %d with %d bytes received! \n", downlinkPayload->portNo, downlinkPayload->len); // Just for Debug
	
	if (1 == downlinkPayload->portNo) // Hvis vi modtager noget på port 1
	{
	feedAnimalTerrarium();
	
	vTaskDelay(pdMS_TO_TICKS(4000));
	
	resetAnimalFeeder();
	}
}

/************************************************************************/
/* loraDownlinkInit
Resetter animal feederen                                                */
/************************************************************************/
inline void loraDownlinkInit()
{	
	resetAnimalFeeder();
}

/*-----------------------------------------------------------*/
void lora_downlink_handler_task( void *pvParameters )
{
	loraDownlinkInit();
		
	for(;;)
	{
		loraDownlinkRun();
	}
}

/************************************************************************/
/* init_downlink_handler
Sætter message buffer og allokerer memory til downlink payloaden        */
/************************************************************************/
void init_downlink_handler(MessageBufferHandle_t downLinkMessageBufferHandle){
	downLinkMessageBuffer = downLinkMessageBufferHandle;
	downlinkPayload = malloc(sizeof(lora_driver_payload_t));
}

