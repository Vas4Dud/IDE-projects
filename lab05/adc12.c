/**
 * ******************************************************************************
 * @file    : adc12.h
 * @brief   : ADC module header file
 * @details : ADC initialization and interaction
 * @note    : ADC does not require IOMUX interaction
 * 
 * @author 
 * @date 
 * ******************************************************************************
*/

#ifndef _ADC12_H_
#define _ADC12_H_

#include <stdint.h>
#include <ti/devices/msp/msp.h>
/**
 * @brief Initialize ADC0
*/
void ADC0_init(void);


/**
 * @brief Retrieve the value from the ADC0
 * @note The ADC channel in use is set during initialization
 *       The channel is not the same as where the module stores the value
 * @return ADC0 processed value
*/
uint32_t ADC0_getVal(void);


#endif // _ADC12_H_

void ADC0_init(void)
{
	if ((ADC0->ULLMEM.GPRCM.PWREN & ADC12_PWREN_ENABLE_ENABLE) 
			!= ADC12_PWREN_ENABLE_ENABLE )
	{
		ADC0->ULLMEM.GPRCM.RSTCTL = ADC12_RSTCTL_KEY_UNLOCK_W | 
												ADC12_RSTCTL_RESETSTKYCLR_CLR;
		ADC0->ULLMEM.GPRCM.PWREN = ADC12_PWREN_KEY_UNLOCK_W |
											ADC12_PWREN_ENABLE_ENABLE;  
	}	
	
	ADC0->ULLMEM.GPRCM.CLKCFG = ADC12_CLKCFG_SAMPCLK_ULPCLK |
															ADC12_CLKCFG_KEY_UNLOCK_W;

	ADC0->ULLMEM.CLKFREQ = ADC12_CLKFREQ_FRANGE_RANGE40TO48;
	
	ADC0->ULLMEM.CTL0 |= ADC12_CTL0_SCLKDIV_DIV_BY_8 |
												ADC12_CTL0_PWRDN_MANUAL;
	ADC0->ULLMEM.CTL1 |= ADC12_CTL1_CONSEQ_SINGLE | 
												ADC12_CTL1_SAMPMODE_AUTO;
}

uint32_t ADC0_getVal(void)
{
	
	ADC0->ULLMEM.GPRCM.PWREN |= ADC12_PWREN_ENABLE_ENABLE;
	ADC0->ULLMEM.CTL1 |= ADC12_CTL1_SC_START;
	while(ADC0->ULLMEM.CTL1 != ADC12_CTL1_SC_STOP);
	ADC0->ULLMEM.CTL0 |= ADC12_CTL0_ENC_OFF;
	
	uint32_t data = ADC0->ULLMEM.MEMRES[0];
	return data;
}
