#ifndef _DC_MOTOR_H_
#define _DC_MOTOR_H_

#include <stdint.h>



void init_dc_motor(uint32_t frequency, double percentDutyCycle);

void dc_forward(double percentDutyCycle);

void dc_backwards(double percentDutyCycle);




#endif // _TIMERS_H_
