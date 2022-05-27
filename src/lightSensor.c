
#include <util/delay.h>
#include "tsl2591.h"
#include "terrarium.h"

//definere call back funtionen s� den kan bruges i init
void tsl2591Callback(tsl2591_returnCode_t rc);

/*
Call back funtion bliver givet med i ops�tning af sensor. 

Denne funtion bliver prim�rt kaldt n�r sensoren har data klar efter fetch_data();
N�r data er klar s� g�r funtionen i "TSL2591_DATA_READY" casen, hvor Lux gettes,
derfter opdateres terrarium med seneste m�ling.
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
Functionen st�r for ops�tning af sensor hvor den f�r call back funtionen.
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
Run functionen, bliver brugt af lightSensorTask og st�r for at s�tte m�ling af lys igang.
N�r m�lingen er sat i gang invoker driveren selv call back med resulatet.

M�linger tages hvert 10 sec.
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
Task oprettet i main og k�re run funtionen.

vTaskDelay p� 3 sec for at vente p� driveren er opsat.
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
