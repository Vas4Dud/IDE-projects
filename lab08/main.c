#include <ti/devices/msp/msp.h>
#include "lab5/adc12.h"
#include "lab2/uart.h"
#include <stdio.h>
#include "lab5/timers.h"
static uint32_t samples[1000];
static unsigned long sample_index = 0;
static uint8_t display_ready = 0;

void collect_sample(){
	uint32_t read_val = ADC0_getVal();
	samples[sample_index] = read_val;
	sample_index++;
	if (sample_index == 1000){
		display_ready = 1;
		sample_index = 0;
	}
}

void calculate_and_display(){
	//find max value and count how many times it occurs
	double bpm = samples[0];//placeholder
	
	char display_str[128];
	snprintf(display_str, 127, "Heart Rate is %f BPM\r\n",bpm);
	UART0_put(display_str);
	display_ready = 0;
}	

void TIMG12_IRQHandler(void){
	if(display_ready == 0){
		collect_sample();
	}
}

int main(){
	ADC0_init();
	TIMG12_init(5000);
	UART0_init();
	UART0_put("I am a heartrate monitor\r\n");
	while(1){
		if(display_ready == 1){
			calculate_and_display();
			display_ready = 0;
	}
	}
}
