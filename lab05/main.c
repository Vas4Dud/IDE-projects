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
	TIMG6_init(12500,199);//80Mhz Busclk/(8*200) = 50kHz. 25000/50kHz = 0.5Hz for each interrupt
	TIMG12_init(5000);//80Mhz/(8) = 10Mhz. 10000/10Mhz = 1kHz for each interrupt
	while(1);
}

void TIMG6_IRQHandler(void){
	uint32_t read_val = ADC0_getVal();
	double degree_cel = (3300-read_val)/10.0;
	double degree_far = (degree_cel * (9.0 / 5.0)) + 32.0;
	UART0_put("Celcius: ");
	UART0_printFloat(degree_cel);
	UART0_put(" Farenheit: ");
	UART0_printFloat(degree_far);
	UART0_put("\r\n");
}

void TIMG12_IRQHandler(void){
	if(sw2_state == 1){
		ms_counter++;
	}
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
