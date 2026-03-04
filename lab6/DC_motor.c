#include <ti/devices/msp/msp.h>
#include "DC_motor.h"
#include "lab6/timers.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>


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
			else if (phase==1){
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
			else if (phase==1){
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
	}
	delay(10);
}




void init_dc_motor(uint32_t period, uint32_t prescaler, double percentDutyCycle){
	//forward driver pin
	TIMA0_PWM_init(1, period, prescaler, percentDutyCycle);
	//reverse driver pin, starts with zero
	TIMA1_PWM_init(2, period, prescaler, 0);
}


void dc_forward(double percentDutyCycle){
	TIMA0_PWM_DutyCycle(1, percentDutyCycle);
	TIMA1_PWM_DutyCycle(2, 0);
}

void dc_backwards(double percentDutyCycle){
	TIMA0_PWM_DutyCycle(1, 0);
	TIMA1_PWM_DutyCycle(2, percentDutyCycle);
}

