#include <ti/devices/msp/msp.h>
#include "DC_motor.h"
#include "lab6/timers.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

void init_dc_motor(uint32_t period, uint32_t prescaler, double percentDutyCycle){
	//forward driver pin
	TIMA0_PWM_init(1, period, prescaler, percentDutyCycle);
	//reverse driver pin, starts with zero
	TIMA1_PWM_init(2, period, prescaler, 0);
}


void dc_forward(double percentDutyCycle){
	TIMA0_PWM_DutyCycle(1, percentDutyCycle);
	TIMA1_PWM_DutyCycle(2, 0);
}

void dc_backwards(double percentDutyCycle){
	TIMA0_PWM_DutyCycle(1, 0);
	TIMA1_PWM_DutyCycle(2, percentDutyCycle);
}

