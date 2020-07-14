# VFD_clock
Clock with VFD display based on STM32 MCU. Displays time, date and temperature.

As a display an old VFD tube IW-18 (IV-18) was used. Device displays time (hours, minutes and seconds), date and temperature. Clock is based on STM32F103C8T6 microcontroller and it was programmed with HAL libraries. To boost voltage to required level I used popular SXX1308 step-up IC.

Sensors used in the project:
RTC - DS3231 I2C real time clock
DS18B20 - 1-Wire temperature sensor 
