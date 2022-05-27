#include <ATMEGA_FreeRTOS.h>
#include "message_buffer.h"

void init_downlink_handler(MessageBufferHandle_t downLinkMessageBufferHandle);
void lora_downlink_handler_task( void *pvParameters );
inline void loraDownlinkInit();
inline void loraDownlinkRun();
