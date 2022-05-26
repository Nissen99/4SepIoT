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

float static light;
int static lightCount;

int8_t static isFed;

SemaphoreHandle_t semaphore;

typedef struct Terrariumdata {
	int16_t  temperature;
	int16_t  humidity;
	uint16_t  co2;
	int8_t isFed;
	uint16_t light;

} Terrariumdata;

void initTerrarium(){
	semaphore = xSemaphoreCreateMutex();
}


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

void updateTerrariumCO2(uint16_t co2val)
{
	xSemaphoreTake(semaphore, portMAX_DELAY);
	co2 += co2val;
	co2Count++;
	xSemaphoreGive(semaphore);
}

void updateTerrariumLight(float lightVal){
	light += lightVal;
}

int16_t getTerrariumTemp(Terrariumdata_p terrariumdata)
{
	return terrariumdata->temperature;
}

int16_t getTerrariumHum(Terrariumdata_p terrariumdata)
{

	return terrariumdata->humidity;
}


uint16_t getTerrariumCO2(Terrariumdata_p terrariumdata)
{
	return terrariumdata->co2;
}

int8_t getTerrariumIsFed(Terrariumdata_p terrariumdata) {
	return terrariumdata->isFed;
}

uint16_t getTerrariumLight(Terrariumdata_p terrariumdata){
	return terrariumdata->light;
}


void feedAnimalTerrarium(){
	xSemaphoreTake(semaphore, portMAX_DELAY);
	printf("Animal feed i Terrarium \n");
	rotateFullyLeft();
	isFed = 1;
	xSemaphoreGive(semaphore);
}

void resetAnimalFeeder(){
	rotateFullyRight();
	printf("Animal feed i Terrarium RESET \n");

}

void resetData(){
	xSemaphoreTake(semaphore, portMAX_DELAY);
	temperature = 0.0;
	temperatureCount = 0;
	humidity = 0.0;
	humidityCount = 0;
	co2 = 0;
	co2Count = 0;
	light = 0.0;
	lightCount = 0;
	isFed = 0;
	xSemaphoreGive(semaphore);
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
	
	if (temperatureCount == 0 || humidityCount == 0 || co2Count == 0){
		printf("Atleast one measurement for each type is required. Returning.");
		free(newTerrarium);
		return NULL;
	} 

	float tempAvg =  temperature/temperatureCount;
	int16_t tempAvgX10 = (int16_t) (tempAvg*10);
	
	
	float humAvg = humidity/humidityCount;
	int16_t humAvgX10 = (int16_t) (humAvg*10);
	
	uint16_t co2Avg = (uint16_t) co2/co2Count;
	
	uint16_t lightAvg = (uint16_t) (light/lightCount);
	
	int8_t isFedInt = isFed;
	
	
	xSemaphoreGive(semaphore);
	resetData();
	newTerrarium->temperature = tempAvgX10;
	newTerrarium->humidity = humAvgX10;
	newTerrarium->co2 = co2Avg;
	newTerrarium->isFed = isFedInt;
	newTerrarium->light = lightAvg;

	return newTerrarium;

}