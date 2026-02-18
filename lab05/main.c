#include <ti/devices/msp/msp.h>
#include "lab5/switches.h"
#include "lab5/camera.h"
#include "lab5/adc12.h"
#include "lab5/timers.h"
#include "lab1/leds.h"
#include "lab2/uart.h"

static int sw1_state = 0;
static int sw2_state_counter = 0;
int main(){
	LED1_init();
	LED2_init();
	S1_init_interrupt();
	S2_init_interrupt();
	TIMG6_init(12500,199);//80Mhz Busclk/(8*200) = 50kHz. 25000/50kHz = 0.5Hz for each interrupt
	TIMG12_init(5000);//80Mhz/(8) = 10Mhz. 10000/10Mhz = 1kHz for each interrupt
	while(1);
}

void TIMG6_IRQHandler(void){
	if (sw1_state == 0){
		LED1_set(LED1_TOGGLE);
	}
}

void TIMG12_IRQHandler(void){
	LED2_set((sw2_state_counter++)%8);//sw2_state_counter is ms counter, modulo by 8 maps it to enum state
}
//GPIO interrupt handler
void GROUP1_IRQHandler(void){
	switch(){
	
	}
}