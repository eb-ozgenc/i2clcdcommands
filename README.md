 # I2C Connected LCD Display for ESP 32
Generic 16x2 LCD Commands with I2C Port Extender HW-61 or PCF8574A I/O expander with  ESP IDF
Uses I2C port of ESP32 and pins 21 for SDA and 22 for SCL
Functions for LCD initialization, sending commands and data, writing numbers and strings included in i2clcdcommands.h. It also includes macros for some commands like moving and blinking cursor.
main.c has an appplication example where "Hello world" is written on first row and "128" is written on second row of the LCD
