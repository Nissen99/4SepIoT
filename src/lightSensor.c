
#include <util/delay.h>
#include "tsl2591.h"
#include "terrarium.h"

//definere call back funtionen så den kan bruges i init
void tsl2591Callback(tsl2591_returnCode_t rc);

/*
Call back funtion bliver givet med i opsætning af sensor. 

Denne funtion bliver primært kaldt når sensoren har data klar efter fetch_data();
Når data er klar så går funtionen i "TSL2591_DATA_READY" casen, hvor Lux gettes,
derfter opdateres terrarium med seneste måling.
*/
void tsl2591Callback(tsl2591_returnCode_t rc)
{
	float _lux;
	switch (rc)
	{
		case TSL2591_DATA_READY:
		rc = tsl2591_getLux(&_lux);

		if ( TSL2591_OK == rc)
		{
			updateTerrariumLight(_lux);
			printf("Lux: %d \n",(int) _lux);
		}
		else if( TSL2591_OVERFLOW == rc )
		{
			printf("Lux overflow - change gain and integration time\n");
		}
		break;
		
		case TSL2591_OK:
		printf("Light Switch: TSL2591_OK \n");
		break;
		
		case TSL2591_BUSY:
		printf("Light sensor busy try again \n");
		break;
		
		default:
		printf("Something wrong callBack and data not ready ENUM: %d \n", rc);
		break;
	}
	
}

/*
initLightSensor kaldes i main.
Functionen står for opsætning af sensor hvor den får call back funtionen.
*/
void initLightSensor(){
	
	if ( TSL2591_OK == tsl2591_initialise(tsl2591Callback) )
	{
		printf("Light Sensor initialised \n");
		} else{
		printf("Light Sensor init failed \n");
	}
	
}

/*
Run functionen, bliver brugt af lightSensorTask og står for at sætte måling af lys igang.
Når målingen er sat i gang invoker driveren selv call back med resulatet.

Målinger tages hvert 10 sec.
*/
inline void lightSensorRun(){
	
	vTaskDelay(pdMS_TO_TICKS(10000));
	
	tsl2591_returnCode_t rc = tsl2591_fetchData();
	if(TSL2591_OK != rc){
		printf("Fetch Data failed Light sensor \n");
		return;
	}

	
	
}

/*
Task oprettet i main og køre run funtionen.

vTaskDelay på 3 sec for at vente på driveren er opsat.
*/
void lightSensorTask(void* pvParameters) {
	
	vTaskDelay(pdMS_TO_TICKS(3000));
	tsl2591_enable();
	
	while(1) {
		lightSensorRun();
		
	}
	printf("Something wrong lightSensorTask FAILED \n");
	vTaskDelete(NULL);
}
