#include "display.h"

/*
 * @brief Outputs character mask on segments
 *
 * Bits in symbol_mask byte represents powered segments in following order:
 * ABCDEFGH
 *
 * @param[in]
 * symbol_mask 8bit mask of GPIO pins states
 */
void dispChar(uint8_t symbol_mask)
{
	SEGMENTS_GPIO_Port->BSRR = ~symbol_mask;
	SEGMENTS_GPIO_Port->BSRR = symbol_mask << 16u;
}

/*
 * @brief Activates displays in VFD tube
 *
 * bits in displays_mask byte represents powered displays in following order:
 * LSB - far right display
 * MSB - far left display
 *
 * @param[in] displays_mask 8bit mask where one bit is 1 and others are 0. Active bit codes number of activated display.
 */
void selectDisplay(uint8_t displays_mask)
{
	DISPLAYS_GPIO_Port->BSRR = ~displays_mask;
	DISPLAYS_GPIO_Port->BSRR = displays_mask << 16u;
}

/*
 * @brief Multiplexer function that is supposed to be used in regular intervals triggered by timer.
 *
 * It iterates through array of masks of characters that will be displayed. In a single cycle only one display is
 * activated. After 8 cycles it starts again from beginning of masks array.
 */
void multiplexerSequence()
{
	static uint8_t i = 0;
	static uint8_t x = 1;
	static uint16_t blink_timer = 0;

	dispChar(tab_to_display[i]);
	if(blink_timer > (0xFFFF / 2))
		selectDisplay(x << i);
	else
		selectDisplay((x << i) & blink_mask);

	++i;
	blink_timer += 64;

	if(i >= 8)
		i = 0;
}

/*
 * @brief Converts ASCII character to corresponding 7 segment display bitmask
 *
 * If character is undefined, it returns empty mask (space).
 *
 * @param[in] inputChar ASCII character
 * @returns 8bit bitmask for 7 segment display
 */

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

/*
 * @brief Converts single digit to bitmask
 *
 * @param[in] digit single digit that will be converted to corresponding mask
 */
uint8_t digitToBitmask(uint8_t digit)
{
	if(digit > 9)
		return 0b00000000;
	switch (digit)
	{
		case 1:
			return mask_1;
		case 2:
			return mask_2;
		case 3:
			return mask_3;
		case 4:
			return mask_4;
		case 5:
			return mask_5;
		case 6:
			return mask_6;
		case 7:
			return mask_7;
		case 8:
			return mask_8;
		case 9:
			return mask_9;
		case 0:
			return mask_0;
		default:
			return 0;
	}
}

/*
 * @brief Converts string to array of bitmasks
 *
 * String has to be null-terminated!
 *
 * @param[in] string pointer to char array
 * @param[in] pos empty spaces to leave in masks array before first character string.
 */
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

/*
 * @brief Displays time in following format: HH-MM-SS
 */
void displayHour(uint8_t hour, uint8_t minute, uint8_t second)
{
	tab_to_display[7] = digitToBitmask(hour / 10);
	tab_to_display[6] = digitToBitmask(hour % 10);
	tab_to_display[5] = mask_hypen;
	tab_to_display[4] = digitToBitmask(minute / 10);
	tab_to_display[3] = digitToBitmask(minute % 10);
	tab_to_display[2] = mask_hypen;
	tab_to_display[1] = digitToBitmask(second / 10);
	tab_to_display[0] = digitToBitmask(second % 10);

	if(hour / 10 == 0)
		tab_to_display[7] = 0b00000000;
}

/*
 * @brief Displays date in following format: DD.MM.YYYY
 */
void displayDate(uint8_t day, uint8_t month, uint8_t year)
{
	tab_to_display[7] = digitToBitmask(day / 10);
	tab_to_display[6] = digitToBitmask(day % 10);
	tab_to_display[5] = digitToBitmask(month / 10);
	tab_to_display[4] = digitToBitmask(month % 10);
	tab_to_display[3] = mask_2;
	tab_to_display[2] = mask_0;
	tab_to_display[1] = digitToBitmask(year / 10);
	tab_to_display[0] = digitToBitmask(year % 10);

	tab_to_display[6] |= mask_dot;
	tab_to_display[4] |= mask_dot;

	if(day / 10 == 0)
			tab_to_display[7] = 0b00000000;
}
