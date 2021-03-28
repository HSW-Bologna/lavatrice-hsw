#ifndef SPI_H_INCLUDED
#define SPI_H_INCLUDED

#include "spi_common/spi_common.h"
#include "SPI/spi_devices/io/MCP23x17/mcp23x17.h"

#define GPIO_RETRO MCP23X17_GPIO_13
#define MCP23X17_ADDR_1 0x40
#define MCP23X17_ADDR_2 0x42

extern spi_driver_t exp_driver;

void spi_devices_init(void);
uint8_t spi1_master_exchange(uint8_t send);
int spi1_exchange(uint8_t *writebuf, uint8_t *readbuf, size_t length, void *data);
int spi1_cs_control(int level);

#endif