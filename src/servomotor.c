
#include "servomotor.h"
#include "rc_servo.h"
#include <stdio.h>

/*
initServoMotor kaldes i main.
Functionen st�r for ops�tning af motor.
*/
void initServoMotor(){
	rc_servo_initialise();
}

/*
s�tter positionen af servoen s� langt til h�jre som den kan
*/
void rotateFullyRight(){
	rc_servo_setPosition(1, 100);
	printf("Servo fully Right Vrooom (engine noise) \n");

}

/*
s�tter positionen af servoen s� langt til venstre som den kan
*/
void rotateFullyLeft(){
	rc_servo_setPosition(1, -100);
	printf("Servo fully Left Vrooom (engine noise) \n");
		
}