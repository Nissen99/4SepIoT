
#include <stdint.h>
#include "tempHumSensor.h"
#include "hih8120.h"
#include "terrarium.h"

/*
initTempHumSensor kaldes i main.
Functionen står for opsætning af sensor.
*/
void initTempHumSensor(){
	
	if(HIH8120_OK !=  hih8120_initialise()) {
		printf("Failed to initialize temperature humidity sensor\n");
		return;
	}
}

/*
Run functionen, bliver brugt af tempHumSensorTask og står for at måle Temperatur og Humidity,
og derefter opdaterer dem i terrarium.h.

vTaskDelay's efter hih8120_wakeup og hih8120_measure følger anbefalingerne fra hih8120 sensor dokumentation.

Målinger tages hvert 10 sec.
og sker i 4 steps:

1. hih8120_wakeup for at klargøre sensoren.

2. hih8120_measure for at udføre målingen.

3. hih8120_getTemperature og hih8120_getHumidity hvor resultatet af målingen gemmes.

4. updateTerrariumTemperature og updateTerrariumHumidity hvor resultaterne opdateres i terrarium.
*/
inline void tempHumSensorRun()
{
	vTaskDelay(pdMS_TO_TICKS(10000));
	
//step 1
	int returnCode = hih8120_wakeup();
	if(returnCode!= HIH8120_OK) {
		printf("Temperature wakeup error: %d\n", returnCode);
		return;
	}

	vTaskDelay(100);
//step 2
	returnCode = hih8120_measure();
	if (returnCode != HIH8120_OK) {
		printf("Temperature measure error: %d\n", returnCode);
		return;
	}
	vTaskDelay(5);

	
//step 3
	float temperature = hih8120_getTemperature();
	float humidity =  hih8120_getHumidity();


//step 4
	updateTerrariumTemperature(temperature);
	updateTerrariumHumidity(humidity);
		
	printf("Temperature: %dC° \n",(int)temperature);
	printf("Humidity: %d%% \n",(int)humidity);
}

/*
Task oprettet i main og køre run funtionen.
*/
void tempHumSensorTask(void* pvParameters) {

	while(1) {
		tempHumSensorRun();	
	}
	
	printf("Something wrong tempHumSensorTask FAILED \n");
	vTaskDelete(NULL);
}
