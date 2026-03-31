#include <ti/devices/msp/msp.h>
#include "lab5/adc12.h"
#include "lab2/uart.h"
#include <stdio.h>
#include "lab5/timers.h"

#define MIN_INTERVAL 250  // 250 ms = 240 BPM max

static uint32_t previous_read = 0;
static int previous_slope = 0;
static uint32_t prev_peak_time = 0;
static volatile uint32_t current_time = 0;
static volatile double bpm = 0.0;

void collect_sample(void);

void collect_sample(){
	current_time++;
	uint32_t read_val = ADC0_getVal();
	int slope = (int)read_val - (int)previous_read;
	if (previous_slope > 0 && slope<=0){//rising -> falling = peak
		uint32_t time_delta = current_time - prev_peak_time;
		if (time_delta > MIN_INTERVAL) {
			bpm = (double)60000/(double)time_delta;
			prev_peak_time = current_time;
		}
	}
	previous_read = read_val;
	previous_slope = slope;
}

static void print_bpm(){
	char display_str[128];
	snprintf(display_str, 127, "Heart Rate is %f BPM\r\n",bpm);
	UART0_put(display_str);
}

void TIMG12_IRQHandler(void){
	collect_sample();
}

int main(){
	ADC0_init();
	TIMG12_init(5000);
	UART0_init();
	UART0_put("I am a heartrate monitor\r\n");
	while(1){
		if(current_time%500 == 0){
			print_bpm();
		}
	}
}
