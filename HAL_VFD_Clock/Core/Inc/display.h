#ifndef INC_DISPLAY_H_
#define INC_DISPLAY_H_

#include "stm32f1xx_hal.h"
#include "main.h"

//GPIO ports of segments and displays pins
#define SEGMENTS_GPIO_Port GPIOA
#define DISPLAYS_GPIO_Port GPIOB

//masks of GPIO pins states for different characters
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
//mask that selects displays that will blink (for setting time and date)
uint8_t blink_mask;

void dispChar(uint8_t symbol_mask);
void selectDisplay(uint8_t displays_mask);
void multiplexerSequence();
uint8_t charToBitmask(char inputChar);
void dispString(const char *string, uint8_t pos);
void displayHour(uint8_t hour, uint8_t minute, uint8_t second);
void displayDate(uint8_t day, uint8_t month, uint8_t year);
void displayTemperature(float temperature);

#endif /* INC_DISPLAY_H_ */
