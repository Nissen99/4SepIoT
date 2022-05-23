/*
* loraWANHandler.c
*
* Created: 12/04/2019 10:09:05
*  Author: IHA
*/
#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>
#include <semphr.h>

#include <lora_driver.h>
#include <status_leds.h>

#include "terrarium.h"

// Parameters for OTAA join - You have got these in a mail from IHA
#define LORA_appEUI "2E20554EE0BE7265"
#define LORA_appKEY "D951DC87A928E70B1C2EDD116E87352F"

static char _out_buf[100];
Terrariumdata_p terrariumdata;

void lora_handler_task( void *pvParameters );

static lora_driver_payload_t _uplink_payload;
//(const  portCHAR *
void lora_handler_create(UBaseType_t lora_handler_task_priority)
{
	xTaskCreate(
	lora_handler_task
	, "LRHand" // A name just for humans
	,  configMINIMAL_STACK_SIZE+200  // This stack size can be checked & adjusted by reading the Stack Highwater
	,  NULL
	,  lora_handler_task_priority  // Priority, with 3 (configMAX_PRIORITIES - 1) being the highest, and 0 being the lowest.
	,  NULL );
}

static void _lora_setup(void)
{
	lora_driver_returnCode_t rc;
	status_leds_slowBlink(led_ST2); // OPTIONAL: Led the green led blink slowly while we are setting up LoRa

	// Factory reset the transceiver
	printf("FactoryReset >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_rn2483FactoryReset()));
	
	// Configure to EU868 LoRaWAN standards
	printf("Configure to EU868 >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_configureToEu868()));

	// Get the transceivers HW EUI
	rc = lora_driver_getRn2483Hweui(_out_buf);
	printf("Get HWEUI >%s<: %s\n",lora_driver_mapReturnCodeToText(rc), _out_buf);

	// Set the HWEUI as DevEUI in the LoRaWAN software stack in the transceiver
	printf("Set DevEUI: %s >%s<\n", _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setDeviceIdentifier(_out_buf)));

	// Set Over The Air Activation parameters to be ready to join the LoRaWAN
	printf("Set OTAA Identity appEUI:%s appKEY:%s devEUI:%s >%s<\n", LORA_appEUI, LORA_appKEY, _out_buf, lora_driver_mapReturnCodeToText(lora_driver_setOtaaIdentity(LORA_appEUI,LORA_appKEY,_out_buf)));

	// Save all the MAC settings in the transceiver
	printf("Save mac >%s<\n",lora_driver_mapReturnCodeToText(lora_driver_saveMac()));

	// Enable Adaptive Data Rate
	printf("Set Adaptive Data Rate: ON >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_setAdaptiveDataRate(LORA_ON)));

	// Set receiver window1 delay to 500 ms - this is needed if down-link messages will be used
	printf("Set Receiver Delay: %d ms >%s<\n", 500, lora_driver_mapReturnCodeToText(lora_driver_setReceiveDelay(500)));

	// Join the LoRaWAN
	uint8_t maxJoinTriesLeft = 10;
	
	do {
		rc = lora_driver_join(LORA_OTAA);
		printf("Join Network TriesLeft:%d >%s<\n", maxJoinTriesLeft, lora_driver_mapReturnCodeToText(rc));

		if ( rc != LORA_ACCEPTED)
		{
			// Make the red led pulse to tell something went wrong
			status_leds_longPuls(led_ST1); // OPTIONAL
			// Wait 5 sec and lets try again
			vTaskDelay(pdMS_TO_TICKS(5000UL));
		}
		else
		{
			break;
		}
	} while (--maxJoinTriesLeft);

	if (rc == LORA_ACCEPTED)
	{
		// Connected to LoRaWAN :-)
		// Make the green led steady
		status_leds_ledOn(led_ST2); // OPTIONAL
	}
	else
	{
		// Something went wrong
		// Turn off the green led
		status_leds_ledOff(led_ST2); // OPTIONAL
		// Make the red led blink fast to tell something went wrong
		status_leds_fastBlink(led_ST1); // OPTIONAL

		// Lets stay here
		while (1)
		{
			taskYIELD();
		}
	}
}

inline void init()
{
	printf("kommer vi her i lora handler task?");
	
	// Hardware reset of LoRaWAN transceiver
	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	// Give it a chance to wakeup
	vTaskDelay(150);

	lora_driver_flushBuffers(); // get rid of first version string from module after reset!

	_lora_setup();

	_uplink_payload.len = 6;
	_uplink_payload.portNo = 2;
}

inline void run(TickType_t xLastWakeTime, const TickType_t xFrequency)
{
	//xTaskDelayUntil( &xLastWakeTime, xFrequency );
	vTaskDelay(1000);
	
	
	xSemaphoreTake(semaphore, portMAX_DELAY);
	
	
	Terrariumdata_p terrariumdata = prepareTerrariumData();
	
	int16_t temp = getTerrariumTemp(terrariumdata);
	int16_t hum = getTerrariumHum(terrariumdata);
	uint16_t co2 = getTerrariumCO2(terrariumdata);

printf("Temp: %d	-	Hum: %d		-	Co2: %d\n", temp, hum, co2);

	_uplink_payload.bytes[0] = temp >> 8;
	_uplink_payload.bytes[1] = temp & 0xFF;
	_uplink_payload.bytes[2] = hum >> 8;
	_uplink_payload.bytes[3] = hum & 0xFF;
	_uplink_payload.bytes[4] = co2 >> 8;
	_uplink_payload.bytes[5] = co2 & 0xFF;

	status_leds_shortPuls(led_ST4);  // OPTIONAL
	printf("Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &_uplink_payload)));
	xSemaphoreGive(semaphore);
}

/*-----------------------------------------------------------*/
void lora_handler_task( void *pvParameters )
{
	init();
	

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(30000UL); // Upload message every 5 minutes (300000 ms)
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		
		run(xLastWakeTime, xFrequency);
		
	}
	

}