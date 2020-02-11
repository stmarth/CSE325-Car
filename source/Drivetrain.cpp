#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
#include "Constants.h"

#include "Drivetrain.h"

Drivetrain::Drivetrain() {
	//Setup servo connected to PTB2
	SIM->SCGC5 |= 1 << 10; // Enable clock gating to Port B.
	PORTB->PCR[2] |= 0b011 << 8; // Set pinmux to use TPM
	SIM->SCGC6 |= 1 << 26; // Enable clock gating to TPM module.
	TPM2->MOD = 960;  //At 48MHz, this should trigger an overflow exactly every 200ms, or at 50Hz, which, according to the internet, is the correct frequency for cheap servos.
	TPM2->CONTROLS[0].CnSC |= 0b1001 << 2;  //Set PWM to pulse low on true, so modulo controls pulse width.
	TPM2->CONF |= 0b11 < 6;  //Allow TPM to run in debug mode.
	this->setTheta(0);  //Make sure steering is neutral at boot

	//Setup motor controller with duty cycle on PTB3 and Direction on PTD3
	PORTB->PCR[3] |= 0b011 << 8; // Set pinmux to use TPM
	TPM2->CONTROLS[1].CnSC |= 0b1001 << 2;  //Set PWM to pulse low on true, so modulo controls duty cycle.
	SIM->SCGC5 |= 1 << 12; // Turn on Port D
	PORTD->PCR[3] = 0b001 << 8; // Set pinmux to use GPIO
	GPIOD->PDDR |= 1<<3;  //Set PTD3 as output
	this->setPercentOutputX(0); //Make wheels are neutral at startup.

	TPM2->SC |= 0b01 << 3;  //Enable timer
}

void Drivetrain::setPercentOutputX(float percent){
	//Handle direction
	float output = percent*DRIVEMULTIPLIER;
	if(percent>0){
		GPIOD->PCOR |= 1<<3;  //Clear reverse bit
	}
	else
	{
		GPIOD->PSOR |= 1<<3;  //Set reverse bit
	}

	TPM2->CONTROLS[1].CnV = int(960*abs(output));  //Set duty cycle
}

void Drivetrain::setTheta(float angle){
	TPM2->CONTROLS[0].CnV = int(((0.5*(angle*STEERINGMULTIPLIER)/STEERINGRANGE)+1.5)*19.2);  //Write proper pulse width
}
