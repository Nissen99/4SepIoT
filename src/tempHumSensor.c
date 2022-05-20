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
	}

	vTaskDelay(100);
	returnCode = hih8120_measure();
	if (returnCode != HIH8120_OK && returnCode != HIH8120_TWI_BUSY) {
		printf("Temperature measure error: %d\n", returnCode);

	}
	vTaskDelay(5);


	xSemaphoreTake(semaphore, portMAX_DELAY);

	float temperature = hih8120_getTemperature();
	updateTerrariumTemperature(temperature);
	
	float humidity =  hih8120_getHumidity();
	updateTerrariumHumidity(humidity);

	printf("Temperatur: %dC° \n",(int)temperature);
	printf("Humidity: %d% \n",(int)humidity);
	xSemaphoreGive(semaphore);

	vTaskDelay(100);
}

void tempHumSensorTask(void* pvParameters) {
	(void)pvParameters;

	//her laver vi vores temperature målinger med 100ms delay
	while(1) {
		run();
		
	}

	vTaskDelete(NULL);
}

//
//float messureTemp()
//{
	//int returnCode = hih8120_wakeup();
	//if(returnCode!= HIH8120_OK && returnCode != HIH8120_TWI_BUSY) {
		//printf("Temperature wakeup error: %d\n", returnCode);
	//}
//
	//vTaskDelay(100);
	//returnCode = hih8120_measure();
	//if (returnCode != HIH8120_OK && returnCode != HIH8120_TWI_BUSY) {
		//printf("Temperature measure error: %d\n", returnCode);
//
	//}
	//vTaskDelay(5);
//
//
	////semaphore:
	////xSemaphoreTake(semaphore, portMAX_DELAY);
//
	//float temperature = hih8120_getTemperature();
	//printf("Temperature: %f C°\n",temperature);
//
	////xSemaphoreGive(semaphore);
//
	//return     temperature;
//}