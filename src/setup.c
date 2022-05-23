
/*
 * setup.c
 *
 * Created: 20-05-2022 18:00:02
 *  Author: Solaiman
 */ 
#include "setup.h"


MessageBufferHandle_t downLinkMessageBufferHandle;




void initializeDownlinkMessageBuffer()
{
	downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(int16_t));
}



