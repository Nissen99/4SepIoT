
/*
 * lightSensor.c
 *
 * Created: 26/05/2022 20:01:42
 *  Author: Mikkel
 */ 
#include <util/delay.h>
#include "tsl2591.h"
#include "terrarium.h"




void tsl2591Callback(tsl2591_returnCode_t rc)
{
	float _lux;
	switch (rc)
	{
		case TSL2591_DATA_READY:
		if ( TSL2591_OK == (rc = tsl2591_getLux(&_lux)) )
		{
			updateTerrariumLight(_lux);
			printf("Lux: %5.4f\n", _lux);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Lux overflow - change gain and integration time\n");
		}
		break;
		
		case TSL2591_OK:
		printf("Light Switch: TSL2591_OK");
		// Last command performed successful
		break;
		
		case TSL2591_BUSY:
		printf("Light sensor busy try again");
		break;
		
		default:
		printf("Something wrong callBack and data not ready ENUM: %d", rc);
		break;
	}
	
}

void lightSensorInit(){
	
	if ( TSL2591_OK == tsl2591_initialise(tsl2591Callback) )
	{
		printf("Light Sensor initialised");
		// Driver initilised OK
		// Always check what tsl2591_initialise() returns
	} else{
		printf("Light Sensor init failed");
	}
	
	tsl2591_enable();
}


inline void lightSensorRun(){
	
	if(TSL2591_OK != tsl2591_fetchData()){
		printf("Fetch Data failed Light sensor");
		return;
	}
	
	vTaskDelay(pdMS_TO_TICKS(10000));
	
	
}

void lightSensorTask(void* pvParameters) {
	(void)pvParameters;
	
	lightSensorInit();
	
	while(1) {
		lightSensorRun();
		
	}

	vTaskDelete(NULL);
}
