#include <ti/devices/msp/msp.h>
#include "lab5/adc12.h"
#include "lab2/uart.h"
#include <stdio.h>
#include "lab5/timers.h"
static unsigned long sample_index = 0;
static uint8_t display_ready = 0;
static uint8_t reset_n = 0;
static int32_t ms_counter = 0;

void collect_sample(){
	while(display_ready == 0){
		uint32_t read_val = ADC0_getVal();
		double read_volt = (((double)read_val*3.3)/4096.0);
		
	}
}

void calculate_and_display(){
	char display_str[128];
	snprintf(display_str, 127, "Heart Rate is %f BPM\r\n",bpm);
	UART0_put(display_str);
	display_ready = 0;
}	

void TIMG12_IRQHandler(void){
	if(reset_n == 1){
		ms_counter++;
	}else{
		ms_counter = 0;
	}
}

int main(){
	ADC0_init();
	TIMG12_init(5000);
	UART0_init();
	UART0_put("I am a heartrate monitor\r\n");
	while(1){
	//collect_sample();
	//if (display_ready == 1){
	//	calculate_and_display();
	//}
	}
}
