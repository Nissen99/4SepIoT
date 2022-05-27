
#include "servomotor.h"
#include "rc_servo.h"
#include <stdio.h>

/*
initServoMotor kaldes i main.
Functionen står for opsætning af motor.
*/
void initServoMotor(){
	rc_servo_initialise();
}

/*
sætter positionen af servoen så langt til højre som den kan
*/
void rotateFullyRight(){
	rc_servo_setPosition(1, 100);
	printf("Servo fully Right Vrooom (engine noise) \n");

}

/*
sætter positionen af servoen så langt til venstre som den kan
*/
void rotateFullyLeft(){
	rc_servo_setPosition(1, -100);
	printf("Servo fully Left Vrooom (engine noise) \n");
		
}