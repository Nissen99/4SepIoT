
/*
 * setup.h
 *
 * Created: 20-05-2022 17:59:54
 *  Author: Solaiman
 */ 


#include <ATMEGA_FreeRTOS.h>
#include <message_buffer.h>
#include "lora_driver.h"


//extern MessageBufferHandle_t downLinkMessageBufferHandle;


MessageBufferHandle_t initializeDownlinkMessageBuffer();
