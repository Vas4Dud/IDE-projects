#include <ti/devices/msp/msp.h>
#include "lab5/switches.h"
#include "lab5/camera.h"
#include "lab5/adc12.h"
#include "lab5/timers.h"
#include "lab1/leds.h"
#include "lab2/uart.h"
#include "uart_extras.h"
#include <string.h>
#include <stdio.h>


static int32_t ms_counter = 0;

int main(){
	UART0_init();
	LED1_init();
	LED2_init();
	ADC0_init();
	S1_init_interrupt();
	S2_init_interrupt();
	Camera_init();
	TIMG12_init(5000);//80Mhz/(8) = 10Mhz.
	while(1);
	while(1){
		if(Camera_isDataReady()){
			UART0_put("-1\r\n");
			uint16_t* temp = Camera_getData();
			for(int i=0; i<128 ;i++){
				UART0_printDec(*temp);
				temp++;
				UART0_put("\r\n");
			}
			UART0_put("-2\r\n");
		}
	}
}


void TIMG12_IRQHandler(void){
		ms_counter++;
}

void print_and_reset_ms_count(){
	UART0_printDec(ms_counter);
	UART0_put("\n\r");
	ms_counter = 0;
}


