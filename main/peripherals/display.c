/******************************************************************************/
/*                                                                            */
/*  HSW snc - Casalecchio di Reno (BO) ITALY                                  */
/*  ----------------------------------------                                  */
/*                                                                            */
/*  modulo: display.c                                                         */
/*                                                                            */
/*      gestione display 240x128 con controller ????                          */
/*                                                                            */
/*  Autore: Sabbiolino (Fabio BARALDI) & Massimo ZANNA                        */
/*                                                                            */
/*  Data  : 02/05/2016      REV  : 00.0                                       */
/*                                                                            */
/*  U.mod.: 09/10/2018      REV  : 01.0                                       */
/*                                                                            */
/******************************************************************************/

#include <string.h>
#include <math.h>
#include <stdio.h>

#include "system.h"
#include "hardwareprofile.h"
#include "display.h"
#include "spi_devices.h"


void display_write_byte(unsigned char data, unsigned char iscommand) {
    DISP_DATA_OUT = (DISP_DATA_OUT & ~DISP_DATA_MASK) | data;
    DISP_CD = iscommand;
    DISP_WRITE = 0;
    DISP_READ = 1;
    DISP_CE = 0;
    //delay_us(1);
    // CD set-up time 100ns
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    DISP_CE = 1;
    DISP_WRITE = 1;
    // CD hold-time 10ns
    Nop();
}

unsigned char check_status() {
    DISP_DATA_TRIS = DISP_DATA_TRIS | DISP_DATA_MASK;
    Nop();
    DISP_CD = 1;
    DISP_WRITE = 1;
    DISP_READ = 0;
    DISP_CE = 0;
    
    // delay_us(1);
    // access time 150ns
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    Nop();
    
    unsigned char status = DISP_DATA_IN & DISP_DATA_MASK;
    DISP_CE = 1;
    DISP_READ = 1;
    DISP_DATA_TRIS = DISP_DATA_TRIS & ~DISP_DATA_MASK;
    
    //chek sta0 and sta1
     if ((status & 0x03) == 0x03)
        return 1;
    return 0;
}

unsigned char send_2B_cmd(unsigned char D1, unsigned char D2, unsigned char cmd) {
    //TODO check order of data MSB LSB
    //TODO check timeout

    while (!check_status());
    display_write_byte(D1, 0);
    while (!check_status());
    display_write_byte(D2, 0);
    while (!check_status());
    display_write_byte(cmd, 1);
    return 1;
}

unsigned char send_1B_cmd(unsigned char data, unsigned char cmd) {
    //TODO check timeout
    while (!check_status());
    display_write_byte(data, 0);
    while (!check_status());
    display_write_byte(cmd, 1);
    return 1;
}

unsigned char send_nodata_cmd(unsigned char cmd) {
    //TODO check timeout
    while (!check_status());
    display_write_byte(cmd, 1);
    return 1;
}

void set_address_pointer(unsigned int address) {
    send_2B_cmd((unsigned char)(address & 0xFF),(unsigned char)(address >> 8),0x24);
}

//NB in alto a sinistra
void set_graphic_home_address(unsigned int address)
{
    send_2B_cmd((unsigned char)(address & 0xFF),(unsigned char)(address >> 8),0x42);
}

void set_graphic_area(unsigned char columns)
{
    send_2B_cmd(columns,0x00,0x43);
}

void set_mode(unsigned char mode)
{
    //TODO non � chiaro se ci sono i dati o no
    //send_1B_cmd(0x00,mode);
    send_nodata_cmd(mode);
}

void set_display_mode(unsigned char mode)
{
    //TODO non � chiaro se ci sono i dati o no
    //send_1B_cmd(0x00,mode);
    send_nodata_cmd(mode);
}

void vram_data_write(unsigned char data,unsigned char increment)
{
    if(increment)  
        send_1B_cmd(data,0xC0);
    else
        send_1B_cmd(data,0xC4);
}

void init_display_driver() {
    DISP_RESET = 0;
    __delay_ms(1);
    DISP_RESET = 1;
//    DISP_LIGHT = 1;
    
    mcp23x17_set_gpio_level(exp_driver, MCP23X17_ADDR_1, GPIO_RETRO, 1);
    set_mode(OR_MODE);
    set_display_mode(TEXT_OFF_GRAPHIC_ON);
    set_graphic_home_address(0x00);
    set_graphic_area(HW_COLUMNS);
}

// row 0-127
// columns 0-29
// data is 8 pizels in a row
void display_graphic_write(unsigned char row,unsigned char col,unsigned char data)
{
    unsigned int address = (unsigned int)row * HW_COLUMNS + (unsigned int)col;
    set_address_pointer(address);
    vram_data_write(data,0);
}

