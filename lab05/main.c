#include <ti/devices/msp/msp.h>
#include "lab5/switches.h"
#include "lab5/camera.h"
#include "lab5/adc12.h"
#include "lab5/timers.h"
#include "lab1/leds.h"
#include "lab2/uart.h"

int main(){
	LED2_init();
	S1_init();
	S2_init();
	return 1;
}
