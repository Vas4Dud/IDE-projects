/**
 * ******************************************************************************
 * @file    : leds.h
 * @brief   : LEDs module header file
 * @details : LED initialization and interaction
 * 
 * @author 
 * @date 
 * ******************************************************************************
*/

#ifndef _LEDS_H_
#define _LEDS_H_

/**
 * @brief Initialze LED1
 * @hint You might want to check out the schematics in the MSP User Guide
 *       The IOMUX has a hardware inversion bit
*/
void LED1_init(void);


/**
 * @brief Initialize LED2
 * @note You must account for each LED color
*/
void LED2_init(void);

enum LED1_states{
	LED1_ON,
	LED1_OFF,
	LED1_TOGGLE
};

/**
 * @brief Set LED1 output state
 * @note ON, OFF, TOGGLE
*/
void LED1_set(enum LED1_states);

enum LED2_states{
	LED2_RED, 
	LED2_GREEN, 
	LED2_BLUE, 
	LED2_CYAN, 
	LED2_MAGENTA, 
	LED2_YELLOW, 
	LED2_WHITE, 
	LED2_OFF
};


/**
 * @brief Set LED2 output state
 * @note RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, OFF
*/
void LED2_set(enum LED2_states);


#endif // _LEDS_H_
