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

#endif /* INC_DISPLAY_H_ */
