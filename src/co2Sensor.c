#include <stdio.h>
#include "co2Sensor.h"
#include "terrarium.h"
#include "mh_z19.h"

static uint16_t ppm;

/*
Run functionen, bliver brugt af co2SensorTask og står for at måle co2,
og derefter opdaterer det i terrarium.h.

Målinger tages hvert 10 sec.
og sker i 3 steps:

1. mh_z19_takeMeassuring for at udføre målingen

2. mh_z19_getCo2Ppm hvor resultatet af målingen gemmes. Dette gemmen gennem en uint16 pointer.

3. updateTerrariumCO2 hvor resultaterne opdateres i terrarium.


*/
inline void co2SensorRun(uint16_t *ppm)
{

	vTaskDelay(pdMS_TO_TICKS(10000));

//step 1
	mh_z19_returnCode_t rc = mh_z19_takeMeassuring();
	if(rc != MHZ19_OK) {
		printf("CO2 sensor FAIL mh_z19_returnCode_t ENUM: %d \n", rc);
		return;
	}
	
	vTaskDelay(100);

//step 2	
	rc = mh_z19_getCo2Ppm(ppm);
	
	if (rc != MHZ19_OK)
	{
		printf("CO2 LÆSNING AF sensor FAIL mh_z19_returnCode_t ENUM: %d \n", rc);
		return;
	}

//step 3
	updateTerrariumCO2(*ppm);

	printf("CO2 level : %d ppm \n", (int) *ppm);

}

/*
co2SensorInit kaldes i main.
Functionen står for opsætning af sensor og pointer som sensoren bruger til målinger.
*/
void co2SensorInit() {
	ppm = pvPortMalloc(sizeof(uint16_t));
	mh_z19_initialise(ser_USART3);
	printf("Co2 Sensor init called \n");
}

/*
Task oprettet i main og køre run funtionen.
*/
void co2SensorTask(void* pvParameters) {
		
	while(1) {
		co2SensorRun(&ppm);
	}
	printf("Something wrong co2 Task FAILED \n");
	vTaskDelete(NULL);
}

