#include <stdio.h>
#include <string.h>
#include <ti/devices/msp/msp.h>
#include <stdlib.h>
#include <lab4\I2C\i2c.h>
#include <lab4\I2C\oled.h>
#include <lab1\switches.h>
/**
 * ******************************************************************************
 * @file    : main.c
 * @brief   : I2C usage c file
 * @details : I2C functionality testing
 * 
 * @author Vassily Dudkin
 * @date 1/28/2026
 * ******************************************************************************
*/
void part1(void);
void part2(void);

int main(void)
{
	S1_init();
	OLED_Init();
	
	part1();
	part2();
}

void part1(void)
{
	char* string1 = "Hello World";
	unsigned int string1_len = strlen(string1);
	char* string2 = "How Are You?";
	unsigned int string2_len = strlen(string2);
	char* string3 = "Goodbye";
	unsigned int string3_len = strlen(string3);
	char* string4 = "Sam and Vassily";
	unsigned int string4_len = strlen(string4);

	for(unsigned int j = 0; j < string1_len; j++){
		OLED_draw(0,(int)j,string1[j]);
	}
	for(unsigned int j = 0; j < string2_len; j++){
		OLED_draw(1,(int)j,string2[j]);
	}
	for(unsigned int j = 0; j < string3_len; j++){
		OLED_draw(2,(int)j,string3[j]);
	}
	for(unsigned int j = 0; j < string4_len; j++){
		OLED_draw(3,(int)j,string4[j]);
	}

}

void part2(void)
{
	int count = 0;
	while(true)
	{
		if (count == 0)
		{
			//display first image
			OLED_draw(2, 2, 'x');
		}
		if(S1_pressed())
		{
			//display a second image
			count++;
			if (count == 1)
			{
				OLED_draw(2, 2, 'b');
			}
			if (count == 2)
			{
				//display third image
				OLED_draw(2, 2, 'z');
			}
			if (count == 3)
			{
				count = 0;
			}
		}
	}
}