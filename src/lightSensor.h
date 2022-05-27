
/*
 * lightSensor.h
 *
 * Created: 26/05/2022 20:01:32
 *  Author: Mikkel
 */ 

void lightSensorTask(void* pvParameters);
inline void lightSensorRun();
void tsl2591Callback(tsl2591_returnCode_t rc);