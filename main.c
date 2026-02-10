#include <stdio.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include <stdlib.h>
#include "lab1\switches.h"
#include "lab4\I2C\i2c.h"
#include "lab4\I2C\oled.h"
#include "lab1\leds.h"
/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : I2C usage c file
 * @details : I2C functionality testing
 * 
 * @author Vassily Dudkin and Samuel Pepperman
 * @date 1/28/2026
 * ******************************************************************************
*/
void part1(void);
void part2(void);

int main(void)
{
	S1_init();
	LED1_init();
	OLED_Init();
	
	part1();
	//part2();
}

void part1(void)
{
	OLED_display_clear();
	char* string1 = "Hello World";
	char* string2 = "How Are You?";
	char* string3 = "Goodbye";
	char* string4 = "Sam and Vassily";

	while(1){
	OLED_display_clear();
	OLED_PrintLine(string1);
	OLED_PrintLine(string2);
	OLED_PrintLine(string3);
	OLED_PrintLine(string4);
		
	for (volatile long i = 0; i < 10000; i++){}
	OLED_display_clear();
	uint16_t line[128];
  for (short i = 0; i<128; i++){
	line[i] = (uint16_t)(10*(i) + 5);
	}		
	OLED_DisplayCameraData(line);
	for (volatile long i = 0; i < 10000; i++){}
	}
}

void part2(void)
{

	int count = 0;
	OLED_display_clear();
	while(true)
	{
		if (count == 0)
		{
			//display first image
			OLED_display_clear();
			OLED_Print(1, 1, "        o       ");
			OLED_Print(2, 1, "    o       o   ");
			OLED_Print(3, 1, "   o  O   O  o  ");
			OLED_Print(4, 1, "   o   ---   o   ");
      count++;
		}
		if(S1_pressed())
		{
			//display a second image
			if (count == 1)
			{
				OLED_display_clear();
				OLED_Print(1, 1, "        o       ");
				OLED_Print(2, 1, "    o       o   ");
				OLED_Print(3, 1, "   o  O   O  o  ");
				OLED_Print(4, 1, "   o   \\\_/  o   ");
			}
			if (count == 2)
			{
				//display third image
				OLED_display_clear();
				OLED_Print(1, 1, "        o       ");
				OLED_Print(2, 1, "    o       o   ");
				OLED_Print(3, 1, "   o  O   O  o  ");
				OLED_Print(4, 1, "   o    O    o   ");
			}

			
			if (count == 3)
			{
				count = 0;
			}
			else
			{
				count++;
			}
		}
	}
}
