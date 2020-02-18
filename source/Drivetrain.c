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

void initDrivetrain() {
	//Setup servo connected to PTB2
	SIM->SCGC5 |= 1 << 10; // Enable clock gating to Port B.
	PORTB->PCR[2] &= ~(0b111<<8);
	PORTB->PCR[2] |= 0b011 << 8; // Set pinmux to use TPM
	SIM->SCGC6 |= 1 << 26; // Enable clock gating to TPM module.
	TPM2->SC|=100;  //Precale down by 16;
	TPM2->MOD = 6000;  //At 48MHz, this should trigger an overflow exactly every 2ms, or at 500Hz, which, according to the internet, is the correct frequency for cheap servos.
	TPM2->CONTROLS[0].CnSC |= 0b1010 << 2;  //Set PWM to pulse low on true, so modulo controls pulse width.
	TPM2->CONF |= 0b11 < 6;  //Allow TPM to run in debug mode.
	setTheta(0);  //Make sure steering is neutral at boot

	//Setup motor controller with duty cycle on PTB3 and Direction on PTD3
	PORTB->PCR[3] &= ~(0b111 << 8);
	PORTB->PCR[3] |= 0b011 << 8; // Set pinmux to use TPM
	TPM2->CONTROLS[1].CnSC |= 0b1010 << 2;  //Set PWM to pulse low on true, so modulo controls duty cycle.

	SIM->SCGC5 |= 1 << 12; // Turn on Port D
	PORTD->PCR[3] &= ~(0b111<<8);
	PORTD->PCR[3] = 0b001 << 8; // Set pinmux to use GPIO
	GPIOD->PDDR |= 1<<3;  //Set PTD3 as output
	setPercentOutputX(0); //Make wheels are neutral at startup.
	setDirection(1);
	setTheta(0);
	TPM2->SC |= 0b01 << 3;  //Enable timer


}

void setPercentOutputX(int percent){
	TPM2->CONTROLS[1].CnV = percent;
}

void setDirection(char forward){
	if(forward){
		GPIOD->PCOR |= 1<<3;  //Clear reverse bit
	}
	else
	{
		GPIOD->PSOR |= 1<<3;  //Set reverse bit
	}
}

void setTheta(int angle){
	TPM2->CONTROLS[0].CnV = 2250 + angle;  //Write proper pulse width

}
