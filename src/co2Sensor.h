#include <stdio.h>
#include <stdint.h>
/*
 * co2Sensor.h
 *
 * Created: 20/05/2022 15:43:33
 *  Author: Mikkel
 */ 
void co2SensorTask(void* pvParameters);
inline void co2SensorRun(uint16_t *ppm);
void co2SensorInit();