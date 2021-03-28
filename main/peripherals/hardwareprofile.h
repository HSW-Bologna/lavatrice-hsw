/******************************************************************************/
/*                                                                            */
/*  HSW snc - Casalecchio di Reno (BO) ITALY                                  */
/*  ----------------------------------------                                  */
/*                                                                            */
/*  modulo: HardwareProfile.h                                                 */
/*                                                                            */
/*      assegnazione pin out, parametri timing,                               */
/*                                                                            */
/*  Autore: Maldus(Mattia MALDINI) & Massimo ZANNA                            */
/*                                                                            */
/*  Data  : 13/11/2020      REV  : 00.0                                       */
/*                                                                            */
/*  U.mod.: 10/03/2021      REV  : 01.0                                       */
/*                                                                            */
/******************************************************************************/

#ifndef HARDWAREPROFILE_H
#define	HARDWAREPROFILE_H

#include <xc.h>



//  I/O pin definitions --------------------------------- //
#define INPUT_PIN                   1
#define OUTPUT_PIN                  0



//  I2C BUS --------------------------------------------- //
#define I2C_CLK                     LATBbits.LATB4 //*
#define I2C_CLK_TRIS                TRISBbits.TRISB4 //*

#define I2C_DATA_OUT                LATAbits.LATA8
#define I2C_DATA_IN                 PORTAbits.RA8
#define I2C_DATA_TRIS               TRISAbits.TRISA8

#define I2C_WP                      LATEbits.LATE15 //*
#define I2C_WP_TRIS                 TRISEbits.TRISE15 //*





//  SPI DEVICE -------------------------------------------------------------- //
#define SPI_HIGH                    1
#define SPI_LOW                     0



// # SPI 1 ===========================================================
#define SPI_1_SCK                   LATFbits.LATF0      // 1 TOUCH & 2 I/O EXP 16b
#define SPI_1_SCK_TRIS              TRISFbits.TRISF0
#define SPI_1_SDO                   LATFbits.LATF1
#define SPI_1_SDO_TRIS              TRISFbits.TRISF1
#define SPI_1_SDI                   PORTBbits.RB10
#define SPI_1_SDI_TRIS              TRISBbits.TRISB10

// IC TOUCH SCREEN
#define CS_TOUCH                    LATDbits.LATD6
#define CS_TOUCH_TRIS               TRISDbits.TRISD6
#define TOUCH_BUSY                  PORTBbits.RB11
#define TOUCH_BUSY_TRIS             TRISBbits.TRISB11

// IC 2 x I/O EXPANDER 16 BIT
#define CS_IO_EXP_16_1_2            LATCbits.LATC9
#define CS_IO_EXP_16_1_2_TRIS       TRISCbits.TRISC9



// # SPI 2 || 2xPT100 # 2xAD-PTC # 2xDA 0/10VDC ======================
#define SPI_2_SCK                   LATAbits.LATA10
#define SPI_2_SCK_TRIS              TRISAbits.TRISA10
#define SPI_2_SDO                   LATBbits.LATB13
#define SPI_2_SDO_TRIS              TRISBbits.TRISB13
#define SPI_2_SDI                   PORTBbits.RB12
#define SPI_2_SDI_TRIS              TRISBbits.TRISB12

//  IC PT100 1
#define SPI_2_CS_PT100A             LATBbits.LATB15
#define SPI_2_CS_PT100A_TRIS        TRISBbits.TRISB15
//  IC PT100 2
#define SPI_2_CS_PT100B             LATEbits.LATE14
#define SPI_2_CS_PT100B_TRIS        TRISEbits.TRISE14
//  IC ADC x 2 PTC
#define SPI_2_CS_AD                 LATAbits.LATA7
#define SPI_2_CS_AD_TRIS            TRISAbits.TRISA7
//  IC DAC x 2 0/10VDC
#define SPI_2_CS_DA                 LATBbits.LATB14
#define SPI_2_CS_DA_TRIS            TRISBbits.TRISB14



// # SPI 3 || X SCHEDA ESPANSIONE ====================================
#define SPI_3_SCK                   LATBbits.LATB1
#define SPI_3_SCK_TRIS              TRISBbits.TRISB1
#define SPI_3_SDO                   LATBbits.LATB0
#define SPI_3_SDO_TRIS              TRISBbits.TRISB0
#define SPI_3_SDI                   PORTAbits.RA1
#define SPI_3_SDI_TRIS              TRISAbits.TRISA1

#define CS_1_SLAVE                  LATAbits.LATA0
#define CS_1_SLAVE_TRIS             TRISAbits.TRISA0
#define CS_2_SLAVE                  LATAbits.LATA11
#define CS_2_SLAVE_TRIS             TRISAbits.TRISA11
#define CS_3_SLAVE                  LATGbits.LATG9
#define CS_3_SLAVE_TRIS             TRISGbits.TRISG9
#define CS_4_SLAVE                  LATGbits.LATG6
#define CS_4_SLAVE_TRIS             TRISGbits.TRISG6





//  DISPLAY pin ----------------------------------------- //
#define DISP_DATA0                  LATCbits.LATC0
#define DISP_DATA0_TRIS             TRISCbits.TRISC0
#define DISP_DATA1                  LATCbits.LATC1
#define DISP_DATA1_TRIS             TRISCbits.TRISC1
#define DISP_DATA2                  LATCbits.LATC2
#define DISP_DATA2_TRIS             TRISCbits.TRISC2
#define DISP_DATA3                  LATCbits.LATC3
#define DISP_DATA3_TRIS             TRISCbits.TRISC3
#define DISP_DATA4                  LATCbits.LATC4
#define DISP_DATA4_TRIS             TRISCbits.TRISC4
#define DISP_DATA5                  LATCbits.LATC5
#define DISP_DATA5_TRIS             TRISCbits.TRISC5
#define DISP_DATA6                  LATCbits.LATC6
#define DISP_DATA6_TRIS             TRISCbits.TRISC6
#define DISP_DATA7                  LATCbits.LATC7
#define DISP_DATA7_TRIS             TRISCbits.TRISC7

#define DISP_DATA_MASK              0x00FF
#define DISP_DATA_TRIS              TRISC
#define DISP_DATA_IN                PORTC
#define DISP_DATA_OUT               LATC

#define DISP_CE                     LATEbits.LATE13
#define DISP_CE_TRIS                TRISEbits.TRISE13
#define DISP_RESET                  LATEbits.LATE12
#define DISP_RESET_TRIS             TRISEbits.TRISE12
#define DISP_WRITE                  LATCbits.LATC13
#define DISP_WRITE_TRIS             TRISCbits.TRISC13
#define DISP_READ                   LATCbits.LATC10
#define DISP_READ_TRIS              TRISCbits.TRISC10
#define DISP_CD                     LATCbits.LATC11
#define DISP_CD_TRIS                TRISCbits.TRISC11



//  GENERIC INPUTS ------------------------------------------------ //
#define IN_11                       !PORTBbits.RB8
#define IN_11_TRIS                  TRISBbits.TRISB8
//#define IN_02                       !PORTAbits.RA4
//#define IN_02_TRIS                  TRISAbits.TRISA4
//#define IN_03                       !PORTCbits.RC15     
//#define IN_03_TRIS                  TRISCbits.TRISC15
//#define IN_04                       !PORTDbits.RD8
//#define IN_04_TRIS                  TRISDbits.TRISD8
#define IN_07                       !PORTBbits.RB9
#define IN_07_TRIS                  TRISBbits.TRISB9
#define IN_06                       !PORTDbits.RD5
#define IN_06_TRIS                  TRISDbits.TRISD5
//#define IN_07                       !PORTBbits.RB8 // D
//#define IN_07_TRIS                  TRISBbits.TRISB8
//#define IN_08                       !PORTCbits.RC13
//#define IN_08_TRIS                  TRISCbits.TRISC13

//#define IN_09                       !PORTBbits.RB9 // D
//#define IN_09_TRIS                  TRISBbits.TRISB9
//#define IN_10                       !PORTDbits.RD5 // D
//#define IN_10_TRIS                  TRISDbits.TRISD5
//
//#define IN_11                       !PORTBbits.RB8 // D
//#define IN_11_TRIS                  TRISBbits.TRISB8
//#define BUZZER                      LATEbits.LATE15
//#define BUZZER_TRIS                 TRISEbits.TRISE15



//  GENERIC OUTPUTS ----------------------------------------------- //
#define OUT_01                      LATCbits.LATC12
#define OUT_01_TRIS                 TRISCbits.TRISC12

#define OUT_02                      7
//#define OUT_02_TRIS                 TRISFbits.TRISF0
#define OUT_03                      6
//#define OUT_03_TRIS                 TRISFbits.TRISF1
#define OUT_04                      5
//#define OUT_04_TRIS                 TRISBbits.TRISB13

#define OUT_05                      9
//#define OUT_05_TRIS                 TRISBbits.TRISB12
#define OUT_06                      LATCbits.LATC15
#define OUT_06_TRIS                 TRISCbits.TRISC15
#define OUT_07                      11
//#define OUT_07_TRIS                 TRISBbits.TRISB10

#define OUT_08                      12
//#define OUT_08_TRIS                 TRISAbits.TRISA10
#define OUT_09                      13
//#define OUT_09_TRIS                 TRISAbits.TRISA7
#define OUT_10                      14
//#define OUT_10_TRIS                 TRISBbits.TRISB14
#define OUT_11                      15
//#define OUT_11_TRIS                 TRISBbits.TRISB15

#define OUT_12                      LATAbits.LATA9      // PWM 1
#define OUT_12_TRIS                 TRISAbits.TRISA9
#define OUT_13                      LATAbits.LATA4      // PWM 2
#define OUT_13_TRIS                 TRISAbits.TRISA4

#define OUT_PWM_1                   OUT_12
#define OUT_PWM_2                   OUT_13


//#define OUT_08                      LATCbits.LATC12
//#define OUT_08_TRIS                 TRISCbits.TRISC12


//  "APPLICATION INPUTS/OUTPUTS" ---------------------------------- //
#define INT0_PIN                    PORTBbits.RB7
#define INT0_PIN_TRIS               TRISBbits.TRISB7    // INT0 usato per CP PW-OFF

//#define EXT_RESET                   PORTAbits.RA0
//#define EXT_RESET_TRIS              TRISAbits.TRISA0    // out su RESET SUPERVISOR

#define LED_RUN                     LATCbits.LATC8      // DIR
#define LED_RUN_TRIS                TRISCbits.TRISC8


#endif  /* HARDWAREPROFILE_H */

