#include "terrarium.h"
#include "servomotor.h"

float static temperature, humidity, light;
uint16_t static co2;
int8_t static isFed;
int static temperatureCount, humidityCount, co2Count, lightCount;

SemaphoreHandle_t semaphore;

typedef struct Terrariumdata {
	int16_t  temperature;
	int16_t  humidity;
	uint16_t  co2;
	int8_t isFed;
	uint16_t light;
} Terrariumdata;

/************************************************************************/
/* initTerrarium
Opretter semaphore                                                      */
/************************************************************************/
void initTerrarium(){
	semaphore = xSemaphoreCreateMutex();
}

/************************************************************************/
/* updateTerrariumTemperature
Sørger for at opdatere temperatur og temperatureCount, ved at tage og 
frigive sempahoren, for at sikre at der ikke bliver arbejdet simultant 
på samme ressource														*/
/************************************************************************/
void updateTerrariumTemperature(float temp)
{
	xSemaphoreTake(semaphore, portMAX_DELAY);
	temperature += temp;
	temperatureCount++;
	xSemaphoreGive(semaphore);
}

/************************************************************************/
/* updateTerrariumHumidity
Sørger for at opdatere humidity og humidityCount, ved at tage og
frigive sempahoren, for at sikre at der ikke bliver arbejdet simultant
på samme ressource														*/
/************************************************************************/
void updateTerrariumHumidity(float hum)
{
	xSemaphoreTake(semaphore, portMAX_DELAY);
	humidity += hum;
	humidityCount++;
	xSemaphoreGive(semaphore);
}

/************************************************************************/
/* updateTerrariumCO2
Sørger for at opdatere CO2 og CO2Count, ved at tage og
frigive sempahoren, for at sikre at der ikke bliver arbejdet simultant
på samme ressource														*/
/************************************************************************/
void updateTerrariumCO2(uint16_t co2val)
{
	xSemaphoreTake(semaphore, portMAX_DELAY);
	co2 += co2val;
	co2Count++;
	xSemaphoreGive(semaphore);
}

/************************************************************************/
/* updateTerrariumLight
Sørger for at opdatere Light og LightCount, ved at tage og
frigive sempahoren, for at sikre at der ikke bliver arbejdet simultant
på samme ressource														*/
/************************************************************************/
void updateTerrariumLight(float lightVal)
{
	xSemaphoreTake(semaphore, portMAX_DELAY);
	light += lightVal;
	lightCount++;
	xSemaphoreGive(semaphore);
}

/************************************************************************/
/* feedAnimalTerrarium
Beder servomotoren om at roterer, og tager semaphoren for at sætte isFed*/
/************************************************************************/
void feedAnimalTerrarium(){
	printf("Animal feed i Terrarium \n");
	rotateFullyLeft();
	xSemaphoreTake(semaphore, portMAX_DELAY);
	isFed = 1;
	xSemaphoreGive(semaphore);
}

/************************************************************************/
/* resetAnimalFeeder
Beder servomoteren om at roterer for at resette positionen              */
/************************************************************************/
void resetAnimalFeeder(){
	rotateFullyRight();
	printf("Animal feed i Terrarium RESET \n");
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

int8_t getTerrariumIsFed(Terrariumdata_p terrariumdata) 
{
	return terrariumdata->isFed;
}

uint16_t getTerrariumLight(Terrariumdata_p terrariumdata)
{
	return terrariumdata->light;
}

/************************************************************************/
/* resetData
Resetter alt data for at gøre klar til ny datapakke                     */
/************************************************************************/
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
	printf("Terrarium data reset \n");
}

/************************************************************************/
/* prepareTerrariumData
Allokerer memory til oprettelsen af nyt terrarium. Den tager semaphoren
og udregner gennemsnittet af alle målinger. Tilføjer det udregnede data 
i det nye terrarium og returnerer det                                   */
/************************************************************************/
Terrariumdata_p prepareTerrariumData()
{
	Terrariumdata_p newTerrarium = pvPortMalloc(sizeof(Terrariumdata));
	if(newTerrarium == NULL)
	{
		printf("Could not allocate memory in terrarium.c \n" );
		return NULL;
	}
	
	xSemaphoreTake(semaphore, portMAX_DELAY);
	
	if (temperatureCount == 0 || humidityCount == 0 || co2Count == 0 || light == 0)
	{
		printf("Atleast one measurement for each type is required. Returning. \n");
		free(newTerrarium);
		newTerrarium = NULL;
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