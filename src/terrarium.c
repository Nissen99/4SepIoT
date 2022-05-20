/*
 * terrarium.c
 *
 * Created: 20/05/2022 13:24:56
 *  Author: Mikkel
 */ 
#include "terrarium.h"

float static temperature;
int static temperatureCount;

float static humidity;
int static humidityCount;


typedef struct Terrariumdata {
	int16_t  temperature;
	int16_t  humidity;

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
	
	newTerrarium->temperature = tempAvgX10;
	newTerrarium->humidity = humAvgX10;

	return newTerrarium;

}