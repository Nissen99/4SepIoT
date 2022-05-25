
/*
 * co2Sensor.c
 *
 * Created: 20/05/2022 15:44:05
 *  Author: Mikkel
 */ 
#include "co2Sensor.h"
#include "terrarium.h"
#include "mh_z19.h"

static uint16_t ppm;

inline void run(uint16_t *ppm)
{

	mh_z19_returnCode_t rc = mh_z19_takeMeassuring();
	if(rc != MHZ19_OK) {
		printf("CO2 sensor FAIL: %d", rc);
		return;
	}
	
	vTaskDelay(100);
	
	rc = mh_z19_getCo2Ppm(ppm);
	
	if (rc != MHZ19_OK)
	{
		printf("CO2 LÆSNING AF sensor FAIL: %d", rc);
		return;
	}
	

	
	updateTerrariumCO2(*ppm);
	

	printf("CO2 level : %d ppm \n", (int) *ppm);

  vTaskDelay(500);

}

inline void init() {
	ppm = pvPortMalloc(sizeof(uint16_t));
}


void co2SensorTask(void* pvParameters) {
	(void)pvParameters;
	
	init();
	
	//her laver vi vores temperature målinger med 100ms delay
	while(1) {
		run(&ppm);
		
	}

	vTaskDelete(NULL);
}

