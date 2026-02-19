#include <ti/devices/msp/msp.h>
#include "lab5/camera.h"
#include "lab5/adc12.h"
#include "lab5/timers.h"
static uint8_t cameraData_complete = 0;//0 = not ready, 1 = ready
static uint32_t cameraData[128];
static unsigned pixel_counter = 0;

/**
 * @brief Initialize camera associated components
*/
void Camera_init(void){
	//enable GPIO A Peripheral
	if(!(GPIOA->GPRCM.PWREN & GPIO_PWREN_ENABLE_ENABLE)){
		//reset peripheral
		GPIOA->GPRCM.RSTCTL |= (GPIO_RSTCTL_KEY_UNLOCK_W | GPIO_RSTCTL_RESETASSERT_ASSERT);
		//enable peripheral
		GPIOA->GPRCM.PWREN |= (GPIO_PWREN_KEY_UNLOCK_W | GPIO_PWREN_ENABLE_ENABLE);
	}
	//configure GPIOA 28 and 12
	
	//ADC Init
	ADC0_init();
	//TIMG0 init at 100kHz. 40Mhz, clk div 8, prescale 50 = (40*10^6)/8*50 = 100kHz clk 
	TIMG0_init(1,49);
	//TIMG6 init at integration time
	
}


/**
 * @brief Checks whether camer data is ready to retrieve
 * @note Make sure to check all data is available
 * @return True(1)/False(0) if camera data is ready
*/
uint8_t Camera_isDataReady(void){
	return cameraData_complete;
}


/**
 * @brief Retrieves pointer to camera data array
 * @return Pointer to global data array stored locally in this file
*/
uint16_t* Camera_getData(void){
	return cameraData;
}

void TIMG6_IRQHandler(void){
	//ensure clk timer disabled
	if(TIMG0->COUNTERREGS.CTRCTL & GPTIMER_CTRCTL_EN_ENABLED){
		TIMG0->COUNTERREGS.CTRCTL &= ~(GPTIMER_CTRCTL_EN_ENABLED);//disable timer
	}
	//check if there is no data to process
	if (cameraData_complete == 1){
		return;
	}
	//Toggle SI and CLK
	
	
	//enable clk timer
	TIMG0->COUNTERREGS.CTRCTL |= GPTIMER_CTRCTL_EN_ENABLED;
}


void TIMG0_IRQHandler(void){
	//pulse clk pin
	
	//read ADC0
	uint32_t read_val = ADC0_getVal();
	//Populate cameraData and incriment index
	cameraData[pixel_counter] = read_val;
	pixel_counter++;
	//If data is full
	if (pixel_counter >= 128){
		cameraData_complete = 1;
		//disable clk timer
		TIMG0->COUNTERREGS.CTRCTL &= ~(GPTIMER_CTRCTL_EN_ENABLED);
		//reset index
		pixel_counter = 0;
	}
}
