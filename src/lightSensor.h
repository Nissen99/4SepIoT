/*
 * lightSensor.h
 *
 * Created: 26-05-2022 16:46:28
 *  Author: Solaiman
 */ 

#include <stdio.h>
#include <stdint.h>


void lightSensorTask(void* pvParameters);
inline void lightSensorRun(int16_t *lux);
inline void  lightSensorInit();