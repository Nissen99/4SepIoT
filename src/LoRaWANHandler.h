/*
 * LoRaWANHandler.h
 *
 * Created: 20/05/2022 13:23:06
 *  Author: Mikkel
 */ 

void _lora_setup(void);
void lora_handler_task(void* pvParamters);

SemaphoreHandle_t semaphore;
