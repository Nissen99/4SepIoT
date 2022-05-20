/*
 * terrarium.h
 *
 * Created: 20/05/2022 13:25:04
 *  Author: Mikkel
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>



// laver struct
typedef struct Terrariumdata* Terrariumdata_p;

void updateTerrariumTemperature(float temp );

void updateTerrariumHumidity(float hum);

int16_t getTerrariumHum(Terrariumdata_p terrariumdata);

int16_t getTerrariumTemp(Terrariumdata_p terrariumdata);

Terrariumdata_p prepareTerrariumData();

SemaphoreHandle_t semaphore;

