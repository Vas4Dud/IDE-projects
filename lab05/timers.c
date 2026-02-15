/**
 * ******************************************************************************
 * @file    : timers.c
 * @details : Timer initializations
 * 
 * @author Samuel Pepperman
 * @date 2/11/2026
 * ******************************************************************************
*/
#include <ti/devices/msp/msp.h>
#include "lab5/timers.h"

/**
 * @brief Timer G0 module initialization. General purpose timer
 * @note Timer G0 is in Power Domain 0. Check page 3 of the Data Sheet
*/
void TIMG0_init(uint32_t period, uint32_t prescaler){
	//enable peripheral if not enabled
	if(!(TIMG0->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)){
			//assert reset
			TIMG0->GPRCM.RSTCTL |= (GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT);
			//enable
			TIMG0->GPRCM.PWREN |= (GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE);
	}
	//TIMCLK CONFIGURATION
	//Power Domain 0 uses ULPCLK as BUSCLK = 40MHz
	TIMG0->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	//set CLKDIV to 1
	TIMG0->CLKDIV = GPTIMER_CLKDIV_RATIO_DIV_BY_1;
	//set prescaler
	TIMG0->COMMONREGS.CPS = (prescaler & GPTIMER_CPS_PCNT_MASK);
	//Enable TIMCLK
	TIMG0->COMMONREGS.CCLKCTL = GPTIMER_CCLKCTL_CLKEN_ENABLED;
	
	//COUNTING MODE CONTROL
	//set down counting
	TIMG0->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_DOWN;
	//set perdiodic
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_REPEAT_REPEAT_1;
	//counter value after enable
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;
	//Set LOAD to period
	TIMG0->COUNTERREGS.LOAD = period;
	
	//INTERUPT HANDLING
	//disable interupts
	__disable_irq();
	//clear zero event
	TIMG0->CPU_INT.ICLR = GPTIMER_GEN_EVENT1_ICLR_Z_CLR;
	//enable zero event
	TIMG0->CPU_INT.IMASK = GPTIMER_GEN_EVENT0_IMASK_Z_SET;
	//enable the timer
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
	//register NVIC
	NVIC_EnableIRQ(TIMG0_INT_IRQn);
	//enable interrupts
	__enable_irq();
}


/**
 * @brief Timer G6 module initialization. General purpose timer
*/
void TIMG6_init(uint32_t period, uint32_t prescaler){
	//enable peripheral if not enabled
	if(!(TIMG6->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)){
			//assert reset
			TIMG6->GPRCM.RSTCTL |= (GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT);
			//enable
			TIMG6->GPRCM.PWREN |= (GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE);
	}
	//TIMCLK CONFIGURATION
	//Power Domain 1 uses MCLK as BUSCLK = 80MHz
	TIMG6->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	//set CLKDIV to 1
	TIMG6->CLKDIV = GPTIMER_CLKDIV_RATIO_DIV_BY_1;
	//set prescaler
	TIMG6->COMMONREGS.CPS = (prescaler & GPTIMER_CPS_PCNT_MASK);
	//Enable TIMCLK
	TIMG6->COMMONREGS.CCLKCTL = GPTIMER_CCLKCTL_CLKEN_ENABLED;
	
	//COUNTING MODE CONTROL
	//set down counting
	TIMG6->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_DOWN;
	//set perdiodic
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_REPEAT_REPEAT_1;
	//counter value after enable
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;
	//Set LOAD to period
	TIMG6->COUNTERREGS.LOAD = period;
	
	//INTERUPT HANDLING
	//disable interupts
	__disable_irq();
	//clear zero event
	TIMG6->CPU_INT.ICLR = GPTIMER_GEN_EVENT1_ICLR_Z_CLR;
	//enable zero event
	TIMG6->CPU_INT.IMASK = GPTIMER_GEN_EVENT0_IMASK_Z_SET;
	//enable the timer
	TIMG6->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
	//register NVIC
	NVIC_EnableIRQ(TIMG6_INT_IRQn);
	//enable interrupts
	__enable_irq();
}


/**
 * @brief Timer G12 module initialization. General purpose timer
 * @note Timer G12 has no prescaler
*/
void TIMG12_init(uint32_t period){
	//enable peripheral if not enabled
	if(!(TIMG12->GPRCM.PWREN & GPTIMER_PWREN_ENABLE_ENABLE)){
			//assert reset
			TIMG12->GPRCM.RSTCTL |= (GPTIMER_RSTCTL_KEY_UNLOCK_W | GPTIMER_RSTCTL_RESETASSERT_ASSERT);
			//enable
			TIMG12->GPRCM.PWREN |= (GPTIMER_PWREN_KEY_UNLOCK_W | GPTIMER_PWREN_ENABLE_ENABLE);
	}
	//TIMCLK CONFIGURATION
	//uses MCLK as BUSCLK = 80MHz
	TIMG12->CLKSEL = GPTIMER_CLKSEL_BUSCLK_SEL_ENABLE;
	//set CLKDIV to 1
	TIMG12->CLKDIV = GPTIMER_CLKDIV_RATIO_DIV_BY_1;
	//Enable TIMCLK
	TIMG12->COMMONREGS.CCLKCTL = GPTIMER_CCLKCTL_CLKEN_ENABLED;
	
	//COUNTING MODE CONTROL
	//set down counting
	TIMG12->COUNTERREGS.CTRCTL = GPTIMER_CTRCTL_CM_DOWN;
	//set perdiodic
	TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_REPEAT_REPEAT_1;
	//counter value after enable
	TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_CVAE_LDVAL;
	//Set LOAD to period
	TIMG12->COUNTERREGS.LOAD = period;
	
	//INTERUPT HANDLING
	//disable interupts
	__disable_irq();
	//clear zero event
	TIMG12->CPU_INT.ICLR = GPTIMER_GEN_EVENT1_ICLR_Z_CLR;
	//enable zero event
	TIMG12->CPU_INT.IMASK = GPTIMER_GEN_EVENT0_IMASK_Z_SET;
	//enable the timer
	TIMG12->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
	//register NVIC
	NVIC_EnableIRQ(TIMG12_INT_IRQn);
	//enable interrupts
	__enable_irq();
}
