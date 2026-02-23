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
static uint8_t sw1_state = 0;//0 = toggle, 1 = off
static uint8_t sw2_state = 0;//0 = timer off, 1 = timer on
static uint8_t sw2_state_counter = 0;//LED cycle var
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
}

void TIMG12_IRQHandler(void){
		ms_counter++;
}


//GPIO interrupt handler
void GROUP1_IRQHandler(void){
	//clear intterupt
	__NVIC_ClearPendingIRQ(GPIOA_INT_IRQn);//This macro is 1 for both A and B
	//switch 1 IIDX status check
	if(GPIOA->CPU_INT.IIDX & GPIO_GEN_EVENT1_IIDX_STAT_DIO18){
		sw1_state^=1;
	}
	//switch 2 IIDX status check
	if(GPIOB->CPU_INT.IIDX & GPIO_GEN_EVENT1_IIDX_STAT_DIO21){
		sw2_state^=1;//toggle state
		if (sw2_state == 0){//if state was 1 and is now 0, print timer and reset
			UART0_printDec(ms_counter);
			UART0_put("\n\r");
			ms_counter = 0;
			LED2_set(LED2_OFF);
		}else{
			LED2_set((sw2_state_counter++)%7);//since off is enum = 8, modulo 7 cycles it in valid color range
		}
	}
}
