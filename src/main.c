
//library filer
#include <ATMEGA_FreeRTOS.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <semphr.h>
#include <task.h>
#include <serial.h>
#include <time.h>
#include <message_buffer.h>

//include drivers
#include "serial.h"
#include "stdio_driver.h"
#include "lora_driver.h"

//header filer for de task vi opretter og servo.
#include "tempHumSensor.h"
#include "lightSensor.h"
#include "lorawanUplinkHandler.h"
#include "lorawanDownlinkHandler.h"
#include "co2Sensor.h"
#include "terrarium.h"
#include "servomotor.h"

//definere vores min og maks ( vha. FreeRTOS)
#define SENSOR_TASK_PRIORITY (configMAX_PRIORITIES - 3)
#define LORA_TASK_PRIORITY (configMAX_PRIORITIES - 2)

//task setup
TaskHandle_t tempHumSensorHandle = NULL;
TaskHandle_t lorawanUplink = NULL;
TaskHandle_t co2SensorHandle = NULL;
TaskHandle_t loradownlink = NULL;
TaskHandle_t lightHandle = NULL;

//Shared buffer mellem lora uplink og downlink.
MessageBufferHandle_t downLinkMessageBufferHandle;

int main() {
	
	//"Interrupt must be enabled with sei() for light sensor" - Ibs kode siger det
	sei();
	
	// Initialise the trace-driver to be used together with the R2R-Network
	//trace_init(); //IB's kode

	//For at kunne bruge hterm terminalen skal stdio_initialise kaldes. Setting: 57600,8,N,1
	stdio_initialise(ser_USART0);
	
	//buffer til downlink messages, begge handlers skal have den, derfor oprettet i main. 
	downLinkMessageBufferHandle = xMessageBufferCreate(sizeof(lora_driver_payload_t) * 2);
	
	lora_driver_initialise(ser_USART1,downLinkMessageBufferHandle);
	init_downlink_handler(downLinkMessageBufferHandle);

	//init af hardware	
	initServoMotor();
	initTerrarium();
	initTempHumSensor();
	co2SensorInit();
	initLightSensor();
	
	//opretter de Task vi skal lave ( vha. FreeRTOS)
	//Vi giver en tom TaskHandler reference med, for at oprette en reference til den specifikke task - dog bruger vi ikke denne reference
	xTaskCreate(tempHumSensorTask, "Temperature measurement", configMINIMAL_STACK_SIZE, NULL, SENSOR_TASK_PRIORITY, &tempHumSensorHandle);
	xTaskCreate(lora_handler_task, "lorawanUplink", configMINIMAL_STACK_SIZE, NULL,LORA_TASK_PRIORITY, &lorawanUplink);
	xTaskCreate(co2SensorTask, "Co2Mesurement", configMINIMAL_STACK_SIZE, NULL,SENSOR_TASK_PRIORITY, &co2SensorHandle);
	xTaskCreate(lora_downlink_handler_task, "LoraWanDownLink", configMINIMAL_STACK_SIZE, NULL,LORA_TASK_PRIORITY, &loradownlink);
	xTaskCreate(lightSensorTask, "Light Measurement", configMINIMAL_STACK_SIZE, NULL,SENSOR_TASK_PRIORITY, &lightHandle);

	// der må ikke køres kode, når scheduleren er eksekveret
	vTaskStartScheduler();

		while (1)
		{}
}

