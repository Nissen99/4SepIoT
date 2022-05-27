#include <stdio.h>
#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>
#include <stdlib.h>

// laver struct
typedef struct Terrariumdata* Terrariumdata_p;

void updateTerrariumTemperature(float temp );

void updateTerrariumHumidity(float hum);

void updateTerrariumCO2(uint16_t co2);

void updateTerrariumLight(float lightVal);

int16_t getTerrariumHum(Terrariumdata_p terrariumdata);

int16_t getTerrariumTemp(Terrariumdata_p terrariumdata);

uint16_t getTerrariumCO2(Terrariumdata_p terrariumdata);

int8_t getTerrariumIsFed(Terrariumdata_p terrariumdata);

uint16_t getTerrariumLight(Terrariumdata_p terrariumdata);

void feedAnimalTerrarium();

void resetAnimalFeeder();

void resetData();

Terrariumdata_p prepareTerrariumData();

void initTerrarium();
