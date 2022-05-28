#include <ATMEGA_FreeRTOS.h>
#include <lora_driver.h>

#include "terrarium.h"
#include "lorawanUplinkHandler.h"

// Parameters for OTAA join - You have got these in a mail from IHA
#define LORA_appEUI "2E20554EE0BE7265"
#define LORA_appKEY "D951DC87A928E70B1C2EDD116E87352F"

static char _out_buf[100];

static lora_driver_payload_t _uplink_payload;

/************************************************************************/
/* _lora_setup
Sørger for at oprette forbindelse til LoraWan med data fra ønskede arduino shield*/
/************************************************************************/
void _lora_setup(void)
{
	lora_driver_returnCode_t rc;
	
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
	}
	else
	{
		// Something went wrong

		// Lets stay here
		while (1)
		{
			taskYIELD();
		}
	}
}

/************************************************************************/
/* loraHandlerInit
Sørger for at resette LoraWan transceiver og sætter uplink payloaden op */
/************************************************************************/
inline void loraHandlerInit()
{	
	// Hardware reset of LoRaWAN transceiver
	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	// Give it a chance to wakeup
	vTaskDelay(150);

	lora_driver_flushBuffers(); // get rid of first version string from module after reset!

	_lora_setup();

	_uplink_payload.len = 11;
	_uplink_payload.portNo = 2;
}

/************************************************************************/
/* loraHandlerRun
Sørger for at indsamle data fra terrarium og sender uplink til LoraWan  */
/************************************************************************/
inline void loraHandlerRun()
{
	
	vTaskDelay(pdMS_TO_TICKS(300000)); //5 min imellem hver afsendelse af data
	
	Terrariumdata_p terrariumdata = prepareTerrariumData();
	
	if (terrariumdata == NULL)
	{
		printf("Could not allocate memory in terrarium.c \n" );
		return;
	}
	
	//Henter relevant data fra terrarium
	int16_t temp = getTerrariumTemp(terrariumdata);
	int16_t hum = getTerrariumHum(terrariumdata);
	uint16_t co2 = getTerrariumCO2(terrariumdata);
	int8_t isFed = getTerrariumIsFed(terrariumdata);
	uint16_t light = getTerrariumLight(terrariumdata);
	free(terrariumdata);
	terrariumdata = NULL;

	printf("Temp: %d - Hum: %d - Co2: %d - IsFed: %d - Light: %d \n", temp, hum, co2, isFed, light);

	//Udfylder uplink payload'en med data fra terrarium
	_uplink_payload.bytes[0] = temp >> 8;
	_uplink_payload.bytes[1] = temp & 0xFF;
	_uplink_payload.bytes[2] = hum >> 8;
	_uplink_payload.bytes[3] = hum & 0xFF;
	_uplink_payload.bytes[4] = co2 >> 8;
	_uplink_payload.bytes[5] = co2 & 0xFF;
	_uplink_payload.bytes[6] = 0;
	_uplink_payload.bytes[7] = 0;
	_uplink_payload.bytes[8] = light >> 8;
	_uplink_payload.bytes[9] = light & 0xFF;
	_uplink_payload.bytes[10] = isFed;

	printf("Upload Message >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &_uplink_payload)));
}

/*-----------------------------------------------------------*/
void lora_handler_task( void *pvParameters )
{
	loraHandlerInit();
	
	for(;;)
	{
		loraHandlerRun();
	}
}