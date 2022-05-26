/*
 * terrarium.c
 *
 * Created: 20/05/2022 13:24:56
 *  Author: Mikkel
 */ 
#include "terrarium.h"
#include "servomotor.h"

float static temperature;
int static temperatureCount;

float static humidity;
int static humidityCount;

uint16_t static co2;
int static co2Count;

int8_t static isFed;

SemaphoreHandle_t semaphore;

void initTerrarium(){
	semaphore = xSemaphoreCreateMutex();
}

typedef struct Terrariumdata {
	int16_t  temperature;
	int16_t  humidity;
	uint16_t  co2;
	int8_t isFed;

} Terrariumdata;


void updateTerrariumTemperature(float temp )
{
	xSemaphoreTake(semaphore, portMAX_DELAY);
	temperature += temp;
	temperatureCount++;
	xSemaphoreGive(semaphore);

}

void updateTerrariumHumidity(float hum)
{
	xSemaphoreTake(semaphore, portMAX_DELAY);
	humidity += hum;
	humidityCount++;
	xSemaphoreGive(semaphore);

}


int16_t getTerrariumTemp(Terrariumdata_p terrariumdata)
{
	return terrariumdata->temperature;
}

int16_t getTerrariumHum(Terrariumdata_p terrariumdata)
{

	return terrariumdata->humidity;
}

void updateTerrariumCO2(uint16_t co2val)
{
	xSemaphoreTake(semaphore, portMAX_DELAY);
	co2 += co2val;
	co2Count++;	
	xSemaphoreGive(semaphore);
}

uint16_t getTerrariumCO2(Terrariumdata_p terrariumdata)
{
	return terrariumdata->co2;
}

int8_t getTerrariumIsFed(Terrariumdata_p terrariumdata) {
	return terrariumdata->isFed;
}

void feedAnimalTerrarium(){
	xSemaphoreTake(semaphore, portMAX_DELAY);
	printf("Animal feed i Terrarium \n");
	rotate180Servo();
	isFed = 1;
	xSemaphoreGive(semaphore);
}

void resetData(){
	temperature = 0.0;
	temperatureCount = 0;
	humidity = 0.0;
	humidityCount = 0;
	co2 = 0;
	co2Count = 0;
	isFed = 0;
	printf("Data reset \n");

}

Terrariumdata_p prepareTerrariumData()
{

	Terrariumdata_p newTerrarium = pvPortMalloc(sizeof(Terrariumdata));
	if(newTerrarium == NULL)
	{
		printf("could not allocate memory in terrarium.c \n" );
		return NULL;
	}

	printf("succes");
	
	xSemaphoreTake(semaphore, portMAX_DELAY);

	float tempAvg =  temperature/temperatureCount;
	int16_t tempAvgX10 = (int16_t) (tempAvg*10);
	
	float humAvg = humidity/humidityCount;
	int16_t humAvgX10 = (int16_t) (humAvg*10);
	
	uint16_t co2Avg = (uint16_t) co2/co2Count;
	
	int8_t isFedInt = isFed;
	
	resetData();
	xSemaphoreGive(semaphore);
	
	newTerrarium->temperature = tempAvgX10;
	newTerrarium->humidity = humAvgX10;
	newTerrarium->co2 = co2Avg;
	newTerrarium->isFed = isFedInt;

	return newTerrarium;

}