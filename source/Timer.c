#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"

void initTimer(){
	//Set up TPM0
	SIM->SCGC6 |= 1<<24; //Enable TPM Clock Gating
	TPM0->CONF |= 0b11 << 6;  //Enable clock during debugging
	TPM0->MOD = 48000;  //Set clock to throw flags at 1MHz
	TPM0->SC |= 0b01 <<3; //Enable TPM Module
}

void delay(int ms){
	TPM0->CNT = 0;  //Clear counter to set phase
	TPM0->SC |= 1<< 7;  //Clear timer overflow
	for(int i = 0; i<ms; i++){
		TPM0->CNT = 0;  //Clear counter to set phase
		TPM0->SC |= 1<< 7;  //Clear timer overflow
		while(!(TPM0->SC & 1<<7)){
			__asm volatile("nop");
		}
	}
}
