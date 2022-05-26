
/*
 * servomotor.c
 *
 * Created: 20/05/2022 16:48:29
 *  Author: Mikkel
 */ 

#include "servomotor.h"
#include "rc_servo.h"
#include <stdio.h>

void rotate180Servo(){
	
	printf("Servo 180 Wrrruuuuum \n");
	
	
	rc_servo_setPosition(1, -100);
	
		printf("Servo 180 Tilbage  mmmmuuuurrww \n");

		
}
