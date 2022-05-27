#include <stdint.h>

void co2SensorTask(void* pvParameters);
inline void co2SensorRun(uint16_t *ppm_p);
void co2SensorInit();