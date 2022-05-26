/*
* main.c
* Author : IHA
*
* Example main file including LoRaWAN setup
* Just for inspiration :)
*/
//library filer
#include <ATMEGA_FreeRTOS.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <semphr.h>
#include <task.h>
#include <serial.h>
#include <time.h>

//include drivers
#include "hih8120.h"
#include "serial.h"
#include "stdio_driver.h"
#include "lora_driver.h"
#include "mh_z19.h"
#include "rc_servo.h"
#include <message_buffer.h>

//header filer for de task vi opretter
#include "tempHumSensor.h"
#include "LoRaWANHandler.h"
#include "lorawanDownlinkHandler.h"
#include "co2Sensor.h"
#include "terrarium.h"


//definere vores min og maks ( vha. FreeRTOS)
#define TEMP_TASK_PRIORITY (configMAX_PRIORITIES - 3)

//task setup
TaskHandle_t tempHumSensorHandle = NULL;
TaskHandle_t loRaWanHandle = NULL;
TaskHandle_t co2SensorHandle = NULL;
TaskHandle_t loradownlink = NULL;


MessageBufferHandle_t downLinkMessageBufferHandle;

int main() {
	
	
	// Set output ports for leds used in the example
	//DDRA |= _BV(DDA0) | _BV(DDA7);
	
	// Initialise the trace-driver to be used together with the R2R-Network
	trace_init();

	// Make it possible to use stdio on COM port 0 (USB) on Arduino board - Setting 57600,8,N,1
	// for at printe ud skal dette her med ( header filen "stdio_driver.h" skal includes
	stdio_initialise(ser_USART0);
	
	downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t) * 2);
	
	
	// Innitalisere
	
	lora_driver_initialise(ser_USART1,downLinkMessageBufferHandle);
	
	if(HIH8120_OK !=  hih8120_initialise()) {
		printf("Failed to initialize temperature sensor\n");
		return 1;
	}
	initTerrarium();
	
	mh_z19_initialise(ser_USART3);
	rc_servo_initialise();
	rc_servo_setPosition(1, 100);
	init_downlink_handler(downLinkMessageBufferHandle);
	
	
	
	//opretter de Task vi skal lave ( vha. FreeRTOS)
	xTaskCreate(tempHumSensorTask, "Temperature measurement", configMINIMAL_STACK_SIZE, NULL, TEMP_TASK_PRIORITY, &tempHumSensorHandle);
	xTaskCreate(lora_handler_task, "Led", configMINIMAL_STACK_SIZE, NULL,TEMP_TASK_PRIORITY+1, &loRaWanHandle);
	xTaskCreate(co2SensorTask, "co2Mesurement", configMINIMAL_STACK_SIZE, NULL,TEMP_TASK_PRIORITY, &co2SensorHandle);
	xTaskCreate(lora_downlink_handler_task, "loraDownLink", configMINIMAL_STACK_SIZE, NULL,TEMP_TASK_PRIORITY, &loradownlink);


	// der må ikke køres kode, når scheduleren er eksekveret
	vTaskStartScheduler();

		while (1)
		{}

}

