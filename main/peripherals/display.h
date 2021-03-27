#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED


#define OR_MODE                 0x80
#define EXOR_MODE               0x81
#define AND_MODE                0x83

#define TEXT_OFF_GRAPHIC_ON     0x98

#define HW_COLUMNS              30      /* Colonne del buffer "hardware"; il display e' 240x128, quindi ogni riga contiene 30 byte */
#define HW_ROWS                 128     /* Righe del buffer "hardware"; 128 righe, come i pixel */
#define VIRTUAL_ROWS            16      /* Righe del buffer virtuale (o logico); usato quando giriamo lo schermo di 90 o 270 gradi.
                                            Corrisponderebbe a un display 128x240, quindi con 16 byte per ogni rifa del buffer */
#define VIRTUAL_COLUMNS         240     /* Colonne del buffer virtuale (o logico); come sopra, avrebbe 240 righe (come i pixel) */

#define PIXEL_ROWS              240
#define PIXEL_COLUMNS           128


/*----------------------------------------------------------------------------*/
/*  prototipi funzioni                                                        */
/*----------------------------------------------------------------------------*/

void init_display_driver();

// data is 8 pixels in a row
void display_graphic_write(unsigned char row, unsigned char col, unsigned char data);

void vram_data_write(unsigned char data,unsigned char increment);
void set_address_pointer(unsigned int address);

#endif