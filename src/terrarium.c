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

typedef struct Terrariumdata {
	int16_t  temperature;
	int16_t  humidity;
	uint16_t  co2;


} Terrariumdata;



void updateTerrariumTemperature(float temp )
{
	temperature += temp;
	temperatureCount++;

}


void updateTerrariumHumidity(float hum)
{
	humidity += hum;
	humidityCount++;
}



int16_t getTerrariumTemp(Terrariumdata_p terrariumdata)
{

	return terrariumdata->temperature;
}

int16_t getTerrariumHum(Terrariumdata_p terrariumdata)
{

	return terrariumdata->humidity;
}


void updateTerrariumCO2(uint16_t co2)
{
	co2 += co2;
	co2Count++;	
}

uint16_t getTerrariumCO2(Terrariumdata_p terrariumdata)
{
	return terrariumdata->co2;
}

void feedAnimalTerrarium(){
	rotate180Servo();
}


Terrariumdata_p prepareTerrariumData()
{

	Terrariumdata_p newTerrarium = pvPortMalloc(sizeof(Terrariumdata));
	if(newTerrarium == NULL)
	{
		printf("could not allocate memory in terrium.c \n" );
		return NULL;
	}

	printf("succes");


	float tempAvg =  temperature/temperatureCount;
	int16_t tempAvgX10 = (int16_t) tempAvg*10;
	
	float humAvg = humidity/humidityCount;
	int16_t humAvgX10 = (int16_t) humAvg*10;
	
	uint16_t co2Avg = co2/co2Count;
	
	newTerrarium->temperature = tempAvgX10;
	newTerrarium->humidity = humAvgX10;
	newTerrarium->co2 = co2Avg;

	return newTerrarium;

}