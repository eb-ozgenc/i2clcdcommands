#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <rom/ets_sys.h>
#include <driver/i2c.h>


/*  PCF8574A IO expander / HW-61 LCD I2C Module 
    pin connections

    7-4 : LCD data bits 3-0
    3   : Backlight (1 on / 0 off)
    2   : Enable (EN) pin of LCD (high to low pulse needed for clocking)
    1   : RW pin of LCD (0 to write)
    0   : RS pin of LCD (0 to send command, 1 for data) */

#define address 0x27 //standard address for lcds, can change if necessary

void i2cmasterinit(void) {

      i2c_config_t config = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = 21,
        .scl_io_num = 22,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = 100000,};
    i2c_param_config(I2C_NUM_0, &config);
    i2c_driver_install(I2C_NUM_0, config.mode, 0, 0, 0);
}

void i2clcdsenddata (char command) {
    unsigned char high, low;
    low = (command & 0x0f) << 4;
    high = command & 0xf0;

    unsigned char data[4];
    data[0] = high | 0x0d; // high nibble, en =1
    data[1] = high | 0x09; // high nibble, en =0
    data[2] = low | 0x0d; //low nibble, en =1
    data[3] = low | 0x09; // low nibble, en =0
       //rs = 1 in all

    i2c_master_write_to_device(I2C_NUM_0,address , data ,4,1000);   
    ets_delay_us(5000);
}

void i2clcdsendcommand (char command) {
    unsigned char high, low;
    low = (command & 0x0f) << 4;
    high = command & 0xf0;

    unsigned char data[4];
    data[0] = high | 0x0c; // high nibble, en =1
    data[1] = high | 0x08; // high nibble, en =0
    data[2] = low | 0x0c; //low nibble, en =1
    data[3] = low | 0x08; // low nibble, en =0
    //rs = 0 in all

    i2c_master_write_to_device(I2C_NUM_0,address , data ,4,1000); 
    ets_delay_us(5000);
}

void i2clcdinit(void) {
    i2clcdsendcommand(0x28);
    i2clcdsendcommand(0x0C);
    i2clcdsendcommand(0x01);
}

void i2clcdwritestring(char* string){
      while ((*string)!='\0') {
      i2clcdsenddata(*string);
      string++;
      }
}

void i2clcdsendnum(int num) {
    //can send at most 3 digit numbers
    if (num < 9) {
        i2clcdsenddata(num + 0x30);
    }
    else if (num < 99 && num > 9) {
        i2clcdsenddata(num/10 + 0x30);
        i2clcdsenddata(num%10 + 0x30);
    }
    else {
        i2clcdsenddata(num/100 + 0x30);
        i2clcdsenddata((num%100)/10 + 0x30);
        i2clcdsenddata(num%10 + 0x30);
    }
}

void i2clcdsendnum2digit (int num) {
    i2clcdsenddata(num/10 + 0x30);
    i2clcdsenddata(num%10 + 0x30);
    //to be used in send time
}

void i2clcdsendtime(unsigned long time) {
    //input: time in seconds
    //output in xx:yy format
    i2clcdsendnum2digit(time/60);
    i2clcdsenddata(':');
    i2clcdsendnum2digit(time%60);
}

void i2clcdclear(void) {
    i2clcdsendcommand(0x01);}

void i2clcdline1(void) {
    i2clcdsendcommand(0x80);}

void i2clcdline2(void) {
    i2clcdsendcommand(0xC0);}

void i2clcdcursorblink(void) {
    i2clcdsendcommand(0x0E);}

void i2clcdcursoroff(void) {
    i2clcdsendcommand(0x0C);}