#include <ti/devices/msp/msp.h>
#include "DC_motor.h"
#include "lab6/timers.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "sysctl.h"

void delay(int time_ms);

void delay(int time_ms){
	unsigned long cycles_per_ms = SYSCTL_SYSCLK_getMCLK()/1000;
	unsigned long time_to_clk_cycles = (unsigned long)time_ms*cycles_per_ms;
	
	for (volatile unsigned long i=time_to_clk_cycles; i>0; i--);
}


void init_stepper_motor(){
	if(!(GPIOB->GPRCM.PWREN & 1U)){
		//reset peripheral
		GPIOB->GPRCM.RSTCTL |= (GPIO_RSTCTL_KEY_UNLOCK_W | 1U);
		//enable peripheral
		GPIOB->GPRCM.PWREN |= (GPIO_PWREN_KEY_UNLOCK_W | 1U);
	}
	//PB6  = A enable and set output 0
	IOMUX->SECCFG.PINCM[IOMUX_PINCM23] |= (0x80 | 0x01); //set to IO
	IOMUX->SECCFG.PINCM[IOMUX_PINCM23] |= IOMUX_PINCM_DRV_DRVVAL1; //set to high drive strength
	GPIOB->DOESET31_0 |= (1 << 6); //enable output to PB6
	GPIOB->DOUTCLR31_0 |= (1 << 6); //set low
	//PB7  = B enable and set output 0
	IOMUX->SECCFG.PINCM[IOMUX_PINCM24] |= (0x80 | 0x01); //set to IO
	IOMUX->SECCFG.PINCM[IOMUX_PINCM24] |= IOMUX_PINCM_DRV_DRVVAL1; //set to high drive strength
	GPIOB->DOESET31_0 |= (1 << 7); //enable output to PB7
	GPIOB->DOUTCLR31_0 |= (1 << 7); //set low
	//PB0  = C enable and set output 0
	IOMUX->SECCFG.PINCM[IOMUX_PINCM12] |= (0x80 | 0x01); //set to IO
	IOMUX->SECCFG.PINCM[IOMUX_PINCM12] |= IOMUX_PINCM_DRV_DRVVAL1; //set to high drive strength
	GPIOB->DOESET31_0 |= (1 << 0); //enable output to PB0
	GPIOB->DOUTCLR31_0 |= (1 << 0); //set low
	//PB16 = D enable and set output 0
	IOMUX->SECCFG.PINCM[IOMUX_PINCM33] |= (0x80 | 0x01); //set to IO
	IOMUX->SECCFG.PINCM[IOMUX_PINCM33] |= IOMUX_PINCM_DRV_DRVVAL1; //set to high drive strength
	GPIOB->DOESET31_0 |= (1 << 16); //enable output to PB6
	GPIOB->DOUTCLR31_0 |= (1 << 16); //set low
}

void spin_stepper(int forward_true){
	init_stepper_motor();
	int phase = 0;
	//clear all pins
	GPIOB->DOUTCLR31_0 |= (1 << 6);
	GPIOB->DOUTCLR31_0 |= (1 << 7);
	GPIOB->DOUTCLR31_0 |= (1 << 0);
	GPIOB->DOUTCLR31_0 |= (1 << 16);
	while(true){
		if(forward_true){
			if (phase==0){
				GPIOB->DOUTCLR31_0 |= (1 << 16); //set D low
				GPIOB->DOUTSET31_0 |= (1 << 6); //set A high
				phase++;
			}
			else if (phase==1){
				GPIOB->DOUTCLR31_0 |= (1 << 6); //set A low
				GPIOB->DOUTSET31_0 |= (1 << 7); //set B high
				phase++;
			}	
			else if (phase==2){
				GPIOB->DOUTCLR31_0 |= (1 << 7); //set B low
				GPIOB->DOUTSET31_0 |= (1 << 0); //set C high
				phase++;
			}	
			else{
				GPIOB->DOUTCLR31_0 |= (1 << 0); //set C low
				GPIOB->DOUTSET31_0 |= (1 << 16); //set D high
				phase = 0;
			}
		}
		else{
			if (phase==0){
				GPIOB->DOUTCLR31_0 |= (1 << 6); //set A low
				GPIOB->DOUTSET31_0 |= (1 << 16); //set D high
				phase++;
			}
			else if (phase==1){
				GPIOB->DOUTCLR31_0 |= (1 << 16); //set D low	
				GPIOB->DOUTSET31_0 |= (1 << 0); //set C high
				phase++;
			}	
			else if (phase==2){
				GPIOB->DOUTCLR31_0 |= (1 << 0); //set C low
				GPIOB->DOUTSET31_0 |= (1 << 7); //set B high
				phase++;
			}	
			else{
				GPIOB->DOUTCLR31_0 |= (1 << 7); //set B high
				GPIOB->DOUTSET31_0 |= (1 << 6); //set A high
				phase = 0;
			}
		}
	delay(10);
	}
}


int main(){	
	init_stepper_motor();
	init_dc_motor0(10000,0.20);
	while(1);
	double duty_cycle_iter = 0.0;
	while(1){
		//go 0 to 100 forward
		while(duty_cycle_iter<1){
			dc0_forward(duty_cycle_iter);
			duty_cycle_iter+=0.01;
			delay(10);
		}
		//go 100 to 0 forward
		while (duty_cycle_iter>0){
			dc0_forward(duty_cycle_iter);
			duty_cycle_iter-=0.01;
			delay(10);
		}
		
		//go 0 to 100 backwards
		while(duty_cycle_iter<1){
			dc0_backwards(duty_cycle_iter);
			duty_cycle_iter+=0.01;
			delay(10);
		}
		//go 100 to 0 backwards
		while (duty_cycle_iter>0){
			dc0_backwards(duty_cycle_iter);
			duty_cycle_iter-=0.01;
			delay(10);
		}
	}
}
