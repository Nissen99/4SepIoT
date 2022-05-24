
/*
 * setup.c
 *
 * Created: 20-05-2022 18:00:02
 *  Author: Solaiman
 */ 
#include "setup.h"


MessageBufferHandle_t downLinkMessageBufferHandle;






MessageBufferHandle_t initializeDownlinkMessageBuffer()
{
	
	downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t) * 2);
	return downLinkMessageBufferHandle;
}



