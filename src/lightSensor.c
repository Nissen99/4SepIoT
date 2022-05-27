/*
 * lightSensor.c
 *
 * Created: 26-05-2022 16:46:14
 *  Author: Solaiman
 */ 


#include "lightSensor.h"
#include "tsl2591.h"
#include "terrarium.h"



static int16_t lux;






inline void lightSensorRun(int16_t *lux)

{
		int returnCode = tsl2591_enable();
		
		if(returnCode!= TSL2591_OK  && returnCode != TSL2591_BUSY) {
				printf("LightSensor wakeup error: %d\n", returnCode);
				return;
			}
			
			float lightMeasure;
			if(returnCode != TSL2591_DATA_READY) {
				printf("Light sensor not ready\n");
				return;
			}
			if(TSL2591_OK == tsl2591_getLux(&lightMeasure)) {
				
				
				updateTerrariumLight(lightMeasure);
				
				
				printf("Light: %d\n", (uint16_t) lightMeasure);

			}
			else {
				
					printf("Lux overflow\n");	
					
			}
			
 
		//if(TSL2591_OK != tsl2591_fetchData())
		 //{
			//printf("Failed to fetch light data: %d\n");
			//return;
		//}
		//vTaskDelay(100);
		
		
		
		
		
	}


inline void lightSensorInit() {
	lux= pvPortMalloc(sizeof(int16_t));
}


//
//void lightSensorTask(void* pvParameters) {
	//(void)pvParameters;
	//
	//co2SensorInit();
	//
	//while(1) {
		//co2SensorRun(&lux);
		//
	//}
//
	//vTaskDelete(NULL);
//}

