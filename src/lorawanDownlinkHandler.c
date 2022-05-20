
/*
 * lorawanDownlinkHandler.c
 *
 * Created: 20/05/2022 17:18:33
 *  Author: Mikkel
 */ 
#include "lorawanDownlinkHandler.h"
#include "terrarium.h"
#include "message_buffer.h"
#include <lora_driver.h>



	lora_driver_payload_t downlinkPayload;

/*-----------------------------------------------------------*/
void lora_downlink_handler_task( void *pvParameters )
{
		printf("Downlink task startet");

		
	for(;;)
	{
	
		// this code must be in the loop of a FreeRTOS task!
		xMessageBufferReceive(downLinkMessageBufferHandle, &downlinkPayload, sizeof(lora_driver_payload_t), portMAX_DELAY);
		
		printf("DOWN LINK: from port: %d with %d bytes received!", downlinkPayload.portNo, downlinkPayload.len); // Just for Debug
		
		if (1 == downlinkPayload.bytes[0]) // Check that we have got the expected 4 bytes
		{
			// decode the payload into our variales
			feedAnimalTerrarium();
		}
		
		
		
	}

}
