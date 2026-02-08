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
	//enable IC2 module
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
	while(I2C1->MFIFOSR & I2C_MFIFOSR_TXFIFOCNT_MASK == I2C_MFIFOSR_TXFIFOCNT_MINIMUM);//TXFIFOCNT number of Bytes  left into the TX FIFO > 0
	//if my understinding is backwards: 
	//while(I2C1->MFIFOSR & I2C_MFIFOSR_TXFIFOCNT_MASK == I2C_MFIFOSR_TXFIFOCNT_MAXIMUM)
	I2C1->MTXDATA = ch & I2C_MTXDATA_VALUE_MASK;
}

void I2C1_put(unsigned char *data, uint16_t data_size)
{
	//set control register to data size
	uint32_t data_size_processed = (data_size << I2C_MCTR_MBLEN_OFS) & I2C_MCTR_MBLEN_MASK;
	I2C1->MCTR |= (data_size_processed);
	//set direction bit 0
	I2C1->MSA &= ~I2C_MSA_DIR_MASK;
	//enable STOP condition
	I2C1->MCTR |= I2C_MCTR_STOP_ENABLE;
	//enable START/repeated start
	I2C1->MCTR |= I2C_MCTR_START_ENABLE;
	//enable BURSTRUN
	I2C1->MCTR |= I2C_MCTR_BURSTRUN_ENABLE;

	//loop putchar
	for (int i = 0; i<data_size; i++){
		I2C1_putchar(data[i]);
	}
	//wait for status idle			
	while(!(I2C1->MSR & I2C_MSR_IDLE_SET));
	//disable BURSTRUN
	I2C1->MCTR |= I2C_MCTR_BURSTRUN_DISABLE;
}

#endif // _I2C_H_
