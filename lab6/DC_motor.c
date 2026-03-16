#include <ti/devices/msp/msp.h>
#include "DC_motor.h"
#include "lab6/timers.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>

void init_dc_motor(uint32_t frequency, double percentDutyCycle){
	//forward driver pin
	TIMA0_PWM_freq_init(0, frequency, percentDutyCycle);
	//reverse driver pin, starts with zero
	//TIMA1_PWM_init(1, period, prescaler, 0.0);
}


void dc_forward(double percentDutyCycle){
	TIMA0_PWM_DutyCycle(0, percentDutyCycle);
	TIMA1_PWM_DutyCycle(1, 0.0);
}

void dc_backwards(double percentDutyCycle){
	TIMA0_PWM_DutyCycle(0, 0.0);
	TIMA1_PWM_DutyCycle(1, percentDutyCycle);
}

