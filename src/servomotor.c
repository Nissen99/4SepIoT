
/*
 * servomotor.c
 *
 * Created: 20/05/2022 16:48:29
 *  Author: Mikkel
 */ 
#include "servomotor.h"
#include "rc_servo.h"
#include <stdio.h>

void rotateFullyRight(){
	
	printf("Servo Right Wrrruuuuum \n");
	
	rc_servo_setPosition(1, 100);
	
		

		
}


void rotateFullyLeft(){
	printf("Servo Left Wrrruuuuum \n");
	rc_servo_setPosition(1, -100);
	
	
}