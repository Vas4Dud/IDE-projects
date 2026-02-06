/**
 * ******************************************************************************
 * @file    : i2c.c
 * @brief   : I2C module c file
 * @details : I2C initialization and interaction
 * @note    : TI documentation switches terminology around, code is behind
 *             Controller <-> Master
 *             Target     <-> Slave
 *             This changes register access from what the documentation specifies
 *
 * @author 
 * @date 
 * ******************************************************************************
*/

#ifndef _I2C_H_
#define _I2C_H_

#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "sysctl.h"
/**
 * @brief Initialize I2C1
 * @param[in] targetAddress - Passed by OLED file. I2C target/listener address
 * @note OLED file looks for this exact prototype
*/
void I2C1_init(uint16_t targetAddress);


/**
 * @brief Sends a single character byte over I2C1
 * @param[in] ch - Byte to send
*/
void I2C1_putchar(unsigned char ch);


/**
 * @brief Send full character string over I2C until limit is reached
 * @param[in] data - String pointer to data to send
 * @param[in] data_size - Amount of bytes to transmit
*/
void I2C1_put(unsigned char *data, uint16_t data_size);

void I2C1_init(uint16_t targetAddress)
{
	//enable Uart0 module
	if ((I2C1->GPRCM.PWREN & I2C_PWREN_ENABLE_ENABLE) 
			!= I2C_PWREN_ENABLE_ENABLE )
	{
		I2C1->GPRCM.RSTCTL = I2C_RSTCTL_KEY_UNLOCK_W | 
												I2C_RSTCTL_RESETSTKYCLR_CLR;
		I2C1->GPRCM.PWREN = I2C_PWREN_KEY_UNLOCK_W |
											I2C_PWREN_ENABLE_ENABLE;  
	}	
	
	// Enable SCL
	IOMUX->SECCFG.PINCM[IOMUX_PINCM15]|= (IOMUX_PINCM15_PF_I2C1_SCL  | IOMUX_PINCM_PC_MASK
																		| IOMUX_PINCM_INENA_ENABLE);
	
	//Enable SDA
	IOMUX->SECCFG.PINCM[IOMUX_PINCM16]|= (IOMUX_PINCM16_PF_I2C1_SDA 
																			| IOMUX_PINCM_PC_MASK | IOMUX_PINCM_INENA_ENABLE);
	
	//enable bus clock
	I2C1->CLKSEL |= I2C_CLKSEL_BUSCLK_SEL_ENABLE; //ulpclk
	  
	I2C1->CLKDIV |= I2C_CLKDIV_RATIO_DIV_BY_1;
	
	I2C1->GFCTL |= I2C_GFCTL_AGFEN_DISABLE; //disable analog glitch suppression
	
	//clear controller control register
	I2C1->MASTER.MCTR |= I2C_MSA_DIR_OFS;
	
	//run CLK at 400kHz, SCL_LP = 6, SCL_HP = 4, TPR = 0x09
	I2C1->MASTER.MTPR = 0x09;

  I2C1->MASTER.MFIFOCTL = (I2C_MFIFOCTL_RXTRIG_LEVEL_1 |
														I2C_MFIFOCTL_TXTRIG_EMPTY);
												
	I2C1->MASTER.MCR |= I2C_MCR_CLKSTRETCH_DISABLE;
	
	I2C1->MASTER.MSA = targetAddress;
	
	//enable the I2C controller
	I2C1->MASTER.MCR |= I2C_MCR_ACTIVE_ENABLE;
				
}

void I2C1_putchar(unsigned char ch)
{
	
}

void I2C1_put(unsigned char *data, uint16_t data_size)
{
	
}

#endif // _I2C_H_
