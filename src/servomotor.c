
/*
 * servomotor.c
 *
 * Created: 20/05/2022 16:48:29
 *  Author: Mikkel
 */ 

#include "servomotor.h"
#include "rc_servo.h"

void rotate180Servo(){
	
	rc_servo_setPosition(0, 50);
	
	vTaskDelay(1000);
	
	rc_servo_setPosition(0, 0);
		
}
