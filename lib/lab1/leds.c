#include <ti/devices/msp/msp.h>
#include "lab1/leds.h"

void LED1_init(void){
	//enable GPIO A Peripheral
	if(!(GPIOA->GPRCM.PWREN & 1U)){
		//reset peripheral
		GPIOA->GPRCM.RSTCTL |= (GPIO_RSTCTL_KEY_UNLOCK_W | 1U);
		//enable peripheral
		GPIOA->GPRCM.PWREN |= (GPIO_PWREN_KEY_UNLOCK_W | 1U);
		int i = 0;
		while (i < 4){
			i++;//wait ~4 upclock cycles for enable to propogate
		}
	}
	
	//IOMUX config
	IOMUX->SECCFG.PINCM[IOMUX_PINCM1] |= (0x80 | 0x01); //set port to IO
	IOMUX->SECCFG.PINCM[IOMUX_PINCM1] |= IOMUX_PINCM_DRV_DRVVAL1; //set to high drive strength
	GPIOA->DOESET31_0 |= 0x01; //enable output to PA0
	GPIOA->DOUTSET31_0 |= 0x01; //set high since led active low

}

void LED2_init(void){
	//Enable GPIOB
	if(!(GPIOB->GPRCM.PWREN & 1U)){
		//reset peripheral
		GPIOB->GPRCM.RSTCTL |= (GPIO_RSTCTL_KEY_UNLOCK_W | 1U);
		//enable peripheral
		GPIOB->GPRCM.PWREN |= (GPIO_PWREN_KEY_UNLOCK_W | 1U);
		int i = 0;
		while (i < 4){
			i++;//wait ~4 upclock cycles for enable to propogate
		}
	}
	//IOMUX config
	IOMUX->SECCFG.PINCM[IOMUX_PINCM57] |= (0x80 | 0x01); //set red to IO
	IOMUX->SECCFG.PINCM[IOMUX_PINCM57] |= IOMUX_PINCM_DRV_DRVVAL1; //set to high drive strength
	GPIOB->DOESET31_0 |= (1 << 26); //enable output to PB26
	GPIOB->DOUTCLR31_0 |= (1 << 26); //set low since led active high
	
	IOMUX->SECCFG.PINCM[IOMUX_PINCM58] |= (0x80 | 0x01); //set green to IO
	IOMUX->SECCFG.PINCM[IOMUX_PINCM58] |= IOMUX_PINCM_DRV_DRVVAL1; //set to high drive strength
	GPIOB->DOESET31_0 |= (1 << 27); //enable output to PB27
	GPIOB->DOUTCLR31_0 |= (1 << 27); //set low since led active high
	
	IOMUX->SECCFG.PINCM[IOMUX_PINCM50] |= (0x80 | 0x01); //set blue to IO
	IOMUX->SECCFG.PINCM[IOMUX_PINCM50] |= IOMUX_PINCM_DRV_DRVVAL1; //set to high drive strength
	GPIOB->DOESET31_0 |= (1 << 22); //enable output to PB22
	GPIOB->DOUTCLR31_0 |= (1 << 22); //set low since led active high
}

void LED1_set(enum LED1_states new_state){
	switch(new_state){
		case LED1_ON:
			GPIOA->DOUTCLR31_0 |= 0x01;//clear bit since active low
			return;
		case LED1_OFF:
			GPIOA->DOUTSET31_0 |= 0x01;//set bit since active high
			return;
		case LED1_TOGGLE:
			GPIOA->DOUTTGL31_0 |= 0x01;
			return;
	}
}

void LED2_set(enum LED2_states new_state){
	switch(new_state){
		case LED2_OFF:
			//set all low since active high
			GPIOB->DOUTCLR31_0 |= (1 << 26); //set red low
			GPIOB->DOUTCLR31_0 |= (1 << 27); //set green low
		  GPIOB->DOUTCLR31_0 |= (1 << 22); //set blue low	
			return;
		case LED2_RED:
			GPIOB->DOUTSET31_0 |= (1 << 26); //set red high
			GPIOB->DOUTCLR31_0 |= (1 << 27); //set green low
		  GPIOB->DOUTCLR31_0 |= (1 << 22); //set blue low
			return;
		case LED2_GREEN:
			GPIOB->DOUTCLR31_0 |= (1 << 26); //set red low
			GPIOB->DOUTSET31_0 |= (1 << 27); //set green high
		  GPIOB->DOUTCLR31_0 |= (1 << 22); //set blue low	
			return;
		case LED2_BLUE:
			GPIOB->DOUTCLR31_0 |= (1 << 26); //set red low
			GPIOB->DOUTCLR31_0 |= (1 << 27); //set green low
		  GPIOB->DOUTSET31_0 |= (1 << 22); //set blue high	
			return;
		case LED2_CYAN:
			GPIOB->DOUTCLR31_0 |= (1 << 26); //set red low
			GPIOB->DOUTSET31_0 |= (1 << 27); //set green high
		  GPIOB->DOUTSET31_0 |= (1 << 22); //set blue high
			return;
		case LED2_MAGENTA:
			GPIOB->DOUTSET31_0 |= (1 << 26); //set red high
			GPIOB->DOUTCLR31_0 |= (1 << 27); //set green low
		  GPIOB->DOUTSET31_0 |= (1 << 22); //set blue high	
			return;
		case LED2_YELLOW:
			GPIOB->DOUTSET31_0 |= (1 << 26); //set red high
			GPIOB->DOUTSET31_0 |= (1 << 27); //set green high
		  GPIOB->DOUTCLR31_0 |= (1 << 22); //set blue high
			return;
		case LED2_WHITE:
			//turn all on/set high
			GPIOB->DOUTSET31_0 |= (1 << 26); 
			GPIOB->DOUTSET31_0 |= (1 << 27); 
		  GPIOB->DOUTSET31_0 |= (1 << 22); 
			return;
	}
}
