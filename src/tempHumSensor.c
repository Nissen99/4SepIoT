/*
 * temperatureSensor.c
 *
 * Created: 20/05/2022 13:27:01
 *  Author: Mikkel
 */ 
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include <stdio_driver.h>
#include "tempHumSensor.h"
#include "hih8120.h"
#include "terrarium.h"



inline void run()
{
	int returnCode = hih8120_wakeup();
	if(returnCode!= HIH8120_OK && returnCode != HIH8120_TWI_BUSY) {
		printf("Temperature wakeup error: %d\n", returnCode);
		return;
	}

	vTaskDelay(100);
	returnCode = hih8120_measure();
	if (returnCode != HIH8120_OK) {
		printf("Temperature measure error: %d\n", returnCode);
		return;
	}
	vTaskDelay(5);

	
	
	float temperature = hih8120_getTemperature();
	float humidity =  hih8120_getHumidity();



	updateTerrariumTemperature(temperature);
	updateTerrariumHumidity(humidity);
	
	
	printf("Temperature: %dC° \n",(int)temperature);
	printf("Humidity: %d%% \n",(int)humidity);

	vTaskDelay(500);
}

void tempHumSensorTask(void* pvParameters) {

	//her laver vi vores temperature målinger med 100ms delay
	while(1) {
		run();
		
	}

	vTaskDelete(NULL);
}
