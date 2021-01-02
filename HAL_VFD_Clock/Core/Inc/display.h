#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "stm32f1xx_hal.h"
#include "main.h"

#define SEGMENTS_GPIO_Port GPIOA
#define DISPLAYS_GPIO_Port GPIOB

#define mask_A 0b01110111
#define mask_B 0b01111100
#define mask_C 0b00111001
#define mask_D 0b01011110
#define mask_E 0b01111001
#define mask_F 0b01110001
#define mask_H 0b01110110
#define mask_I 0b00000110
#define mask_J 0b00001110
#define mask_L 0b00111000
#define mask_N 0b01010100
#define mask_O 0b01011100
#define mask_P 0b01110011
#define mask_R 0b01010000
#define mask_S 0b01101101
#define mask_T 0b01111000
#define mask_U 0b00011100

#define mask_1 0b00000110
#define mask_2 0b01011011
#define mask_3 0b01001111
#define mask_4 0b01100110
#define mask_5 0b01101101
#define mask_6 0b01111101
#define mask_7 0b00000111
#define mask_8 0b01111111
#define mask_9 0b01101111
#define mask_0 0b00111111

#define mask_degree 0b01100011
#define mask_hypen  0b01000000
#define mask_space  0b00000000
#define mask_dot    0b10000000

//array of bit mask of characters that will be actually displayed on VFD tube
uint8_t tab_to_display[8];

/*
 * bits in symbol_mask byte represents powered segments in following order:
 * ABCDEFGH
 */
void dispChar(uint8_t symbol_mask)
{
	SEGMENTS_GPIO_Port->BSRR = ~symbol_mask;
	SEGMENTS_GPIO_Port->BSRR = symbol_mask << 16u;
}

/*
 * bits in displays_mask byte represents powered displays in following order:
 * LSB - far right display
 * MSB - far left display
 */
void selectDisplay(uint8_t displays_mask)
{
	DISPLAYS_GPIO_Port->BSRR = ~displays_mask;
	DISPLAYS_GPIO_Port->BSRR = displays_mask << 16u;
}

/*
 * Multiplexer function that is supposed to be used in regular intervals triggered by timer.
 */
void multiplexerSequence()
{
	static uint8_t i = 0;
	static uint8_t x = 1;

	dispChar(tab_to_display[i]);
	selectDisplay(x << i);
	i++;

	if(i >= 8)
		i = 0;
}

uint8_t charToBitmask(char inputChar)
{
	switch (inputChar)
	{
		// numbers masks
		case '1':
			return mask_1;
		case '2':
			return mask_2;
		case '3':
			return mask_3;
		case '4':
			return mask_4;
		case '5':
			return mask_5;
		case '6':
			return mask_6;
		case '7':
			return mask_7;
		case '8':
			return mask_8;
		case '9':
			return mask_9;
		case '0':
			return mask_0;

			//special masks
		case ' ':
			return mask_space;
		case '-':
			return mask_hypen;
		case '*':
			return mask_degree;

			//characters masks
		case 'a':
			return mask_A;
		case 'b':
			return mask_B;
		case 'c':
			return mask_C;
		case 'd':
			return mask_D;
		case 'e':
			return mask_E;
		case 'f':
			return mask_F;
		case 'h':
			return mask_H;
		case 'i':
			return mask_I;
		case 'j':
			return mask_J;
		case 'l':
			return mask_L;
		case 'o':
			return mask_O;
		case 'p':
			return mask_P;
		case 'r':
			return mask_R;
		case 's':
			return mask_S;
		case 't':
			return mask_T;
		case 'u':
			return mask_U;
		default:
			return 0;
	}
}

void dispString(const char *string, uint8_t pos)
{
	if(pos > 7)
		return;
	while (*string)
	{
		tab_to_display[pos] = charToBitmask(*string);
		string++;
		pos--;

		if(pos < 0)
			return;
	}
}

void displayHour(uint8_t hour, uint8_t minute, uint8_t second)
{

}

void displayDate(uint8_t day, uint8_t month, uint16_t year)
{

}
#endif /* INC_DISPLAY_H_ */
