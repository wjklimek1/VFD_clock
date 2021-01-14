# VFD_clock
Clock with VFD display based on STM32 MCU. Displays time, date and temperature.

As a display i used an old VFD tube IW-18 (IV-18). Device displays time (hours, minutes and seconds), date and temperature. Clock is based on STM32F103C8T6 microcontroller and it was programmed with HAL libraries. To boost voltage to required level I used popular SX1308 step-up IC.To power VFD display filament with alternating current i used L293d H-bridge with single transistor serving as simple NOT gate.

Schematic and PCB were designed using KiCad. You can find schematic in PDF format in PDFs folder of this repository.

Sensors used in the project:
RTC - DS3231 I2C real time clock

DS18B20 - 1-Wire temperature sensor (not utilized in software yet)
DHT11   - digital relative humidity sensor (not utilized in software yet)
