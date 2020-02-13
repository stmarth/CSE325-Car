#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL46Z4.h"
#include "fsl_debug_console.h"
#include "Drivetrain.h"
#include "Constants.h"
#include "Timer.h"
#include "Assignments.h"

void driveCircle(){
	const int speed = 400;
	const int time = 6450;
	const int theta = 1000;
	setTheta(theta);
	setPercentOutputX(speed);
	delay(time);
	setTheta(0);
	setPercentOutputX(0);
}


void driveSquare(){
	const int speed = 400;
	const int sidetime = 1900;
	const int steertime = 1500;
	const int theta = 1000;
	setPercentOutputX(speed);
	for(int i = 0; i < 4; i++){
		setTheta(0);
		delay(sidetime);
		setTheta(theta);
		delay(steertime);
	}
	setTheta(0);
	setPercentOutputX(0);
}
