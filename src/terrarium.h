/*
 * terrarium.h
 *
 * Created: 20/05/2022 13:25:04
 *  Author: Mikkel
 */ 

#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <stdlib.h>


// laver struct
typedef struct Terrariumdata* Terrariumdata_p;

void updateTerrariumTemperature(float temp );

void updateTerrariumHumidity(float hum);

void updateTerrariumCO2(uint16_t co2);

int16_t getTerrariumHum(Terrariumdata_p terrariumdata);

int16_t getTerrariumTemp(Terrariumdata_p terrariumdata);

uint16_t getTerrariumCO2(Terrariumdata_p terrariumdata);

void feedAnimalTerrarium();

Terrariumdata_p prepareTerrariumData();

SemaphoreHandle_t semaphore;

