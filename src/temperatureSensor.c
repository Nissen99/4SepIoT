/*
 * temperatureSensor.c
 *
 * Created: 20/05/2022 13:27:01
 *  Author: Mikkel
 */ 
#include <ATMEGA_FreeRTOS.h>
#include <stdio.h>
#include <stdio_driver.h>
#include "temperatureSensor.h"
#include "ATMEGA_FreeRTOS.h"
#include "semphr.h"
#include "hih8120.h"



void tempSensorTask(void* pvParameters) {
	(void)pvParameters;

	//her laver vi vores temperature m�linger med 100ms delay
	while(1) {
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


		//semaphore:
		//xSemaphoreTake(semaphore, portMAX_DELAY);

		float temperature = hih8120_getTemperature();
		updateTerrariumTemperature(temperature);

		printf("Temperatur: %fC� \n",temperature);
		//xSemaphoreGive(semaphore);

	}

	vTaskDelete(NULL);
}


float messureTemp()
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


	//semaphore:
	//xSemaphoreTake(semaphore, portMAX_DELAY);

	float temperature = hih8120_getTemperature();
	printf("Temperature: %f C�\n",temperature);

	//xSemaphoreGive(semaphore);

	return     temperature;
}