/*
 * File:   spi_devices.c
 * Author: Virginia
 *
 * Created on 27 marzo 2021, 15.09
 */

#include "hardwareprofile.h"
#include "system.h"
#include "spi_devices/io/MCP23x17/mcp23x17.h"
#include "../peripherals/spi_devices.h"
#include "spi_common/spi_common.h"
#include <stdint.h>

spi_driver_t  exp_driver = SPI_DRIVER(spi1_exchange, spi1_cs_control, NULL);

void spi_devices_init(void) {
    
    size_t i=0;
 
    mcp23x17_init(exp_driver, MCP23X17_ADDR_1, 1);
    mcp23x17_init(exp_driver, MCP23X17_ADDR_2, 1);
    
    for (i = MCP23X17_GPIO_1; i <= MCP23X17_GPIO_16; i++) {
        mcp23x17_set_gpio_direction(exp_driver, MCP23X17_ADDR_1, i, MCP23X17_OUTPUT_MODE);
        mcp23x17_set_gpio_direction(exp_driver, MCP23X17_ADDR_2, i, MCP23X17_OUTPUT_MODE);
    }
    mcp23x17_set_gpio_register(exp_driver, MCP23X17_ADDR_1, 0x0000);
    mcp23x17_set_gpio_register(exp_driver, MCP23X17_ADDR_2, 0);
}

uint8_t spi1_master_exchange(uint8_t send){
    size_t  i;
    uint8_t val, valToSend;
    uint8_t byte = 0;

    for (i = 0; i < 8; i++) {
        valToSend = send & 0x80;
        SPI_1_SDO = valToSend ? SPI_HIGH : SPI_LOW;
        SPI_1_SCK   = SPI_LOW;
        __delay_us(4);
        
        val      = SPI_1_SDI;
        byte = byte | val;

        if (i != 7)
        {
            byte = byte << 1;
            send = send << 1;
        }

        SPI_1_SCK = SPI_HIGH;
        __delay_us(4);
    }
    SPI_1_SDO = SPI_LOW;
    return byte;
}

int spi1_exchange(uint8_t *writebuf, uint8_t *readbuf, size_t length, void *data) {
    int i=0;
    for (i=0; i<length;i++) {
        
        uint8_t byte=0;
        if (writebuf!=NULL) 
            byte=writebuf[i];
        uint8_t receive=spi1_master_exchange(byte);
        if(readbuf!=NULL)
            readbuf[i]=receive;
    }
    return 0;
}

int spi1_cs_control(int level) {
    CS_IO_EXP_16_1_2=level;
    return 0;
}
