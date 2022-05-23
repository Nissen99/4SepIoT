
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


MessageBufferHandle_t downLinkMessageBuffer;
lora_driver_payload_t* downlinkPayload;


/*-----------------------------------------------------------*/
void lora_downlink_handler_task( void *pvParameters )
{
		printf("Downlink task startet");
		
		
	for(;;)
	{
	
		// this code must be in the loop of a FreeRTOS task!
		xMessageBufferReceive(downLinkMessageBuffer, downlinkPayload, sizeof(lora_driver_payload_t), portMAX_DELAY);
		
		
		printf("DOWN LINK: from port: %d with %d bytes received!", downlinkPayload->portNo, downlinkPayload->len); // Just for Debug
		
		
		if (4 == downlinkPayload->len) // Check that we have got the expected 4 bytes
		{
			
		feedAnimalTerrarium();
		
		}
		
	}

}

void init_downlink_handler(MessageBufferHandle_t downLinkMessageBufferHandle){
	downLinkMessageBuffer = downLinkMessageBufferHandle;
	downlinkPayload = malloc(sizeof(lora_driver_payload_t));
}

