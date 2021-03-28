/******************************************************************************/
/*                                                                            */
/*  HSW snc - Casalecchio di Reno (BO) ITALY                                  */
/*  ----------------------------------------                                  */
/*                                                                            */
/*  modulo: system.c                                                          */
/*                                                                            */
/*      inizializzazione REGISTRI micro per START                             */
/*                                                                            */
/*  Autore: Sabbiolino (Fabio BARALDI) e Massimo ZANNA                        */
/*                                                                            */
/*  Data  : 01/05/2016      REV  : 00.0                                       */
/*                                                                            */
/*  U.mod.: 17/11/2020      REV  : 01.0                                       */
/*                                                                            */
/******************************************************************************/

#include "hardwareprofile.h"
#include "system.h"


// FICD
#pragma config ICS = PGD1               // ICD Communication Channel Select bits (Communicate on PGEC1 and PGED1)
#pragma config JTAGEN = OFF             // JTAG Enable bit (JTAG is disabled)

// FPOR
#pragma config ALTI2C1 = OFF            // Alternate I2C1 pins (I2C1 mapped to SDA1/SCL1 pins)
#pragma config ALTI2C2 = OFF            // era ON // Alternate I2C2 pins (I2C2 mapped to ASDA2/ASCL2 pins)
#pragma config WDTWIN = WIN25           // Watchdog Window Select bits (WDT Window is 25% of WDT period)

// FWDT
#pragma config WDTPOST = PS2048         // era PS32768 // Watchdog Timer Postscaler bits (1:32,768)
#pragma config WDTPRE = PR32            // Watchdog Timer Prescaler bit (1:128)
#pragma config PLLKEN = ON              // PLL Lock Enable bit (Clock switch to PLL source will wait until the PLL lock signal is valid.)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable bit (Watchdog Timer in Non-Window mode)
//#pragma config FWDTEN = ON              // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)
#pragma config FWDTEN = OFF             // Watchdog Timer Enable bit (Watchdog timer enabled/disabled by user software)

// FOSC
#pragma config POSCMD = NONE            // Primary Oscillator Mode Select bits (Primary Oscillator disabled)
#pragma config OSCIOFNC = ON            // OSC2 Pin Function bit (OSC2 is general purpose digital I/O pin)
#pragma config IOL1WAY = OFF            // Peripheral pin select configuration (Allow multiple reconfigurations)
//#pragma config FCKSM = CSDCMD           // Clock Switching Mode bits (Both Clock switching and Fail-safe Clock Monitor are disabled)
#pragma config FCKSM = CSECMD           // Clock Switching Mode bits (Clock switching is enabled,Fail-safe Clock Monitor is disabled)

// FOSCSEL
//#pragma config FNOSC = FRCPLL           // Oscillator Source Selection (Fast RC Oscillator with divide-by-N with PLL module (FRCPLL))
//#pragma config IESO = ON                // Two-speed Oscillator Start-up Enable bit (Start up device with FRC, then switch to user-selected oscillator source)
#pragma config FNOSC = FRC              // Oscillator Source Selection (Internal Fast RC (FRC))
#pragma config IESO = OFF               // Two-speed Oscillator Start-up Enable bit (Start up with user-selected oscillator source)

// FGS
#pragma config GWRP = OFF               // General Segment Write-Protect bit (General Segment may be written)
#pragma config GCP = OFF                // General Segment Code-Protect bit (General Segment Code protect is Disabled)

//#pragma config I2CEN = 0                // General Segment Code-Protect bit (General Segment Code protect is Disabled)



void system_oscillator_config(void)
{
    // abilito il PLL e attendo che si stabilizzi
    CLKDIVbits.PLLPRE = 0;
    CLKDIVbits.FRCDIV = 0;
    CLKDIVbits.PLLPOST = 0;
    PLLFBDbits.PLLDIV = 63;
    __builtin_write_OSCCONH(0x01);
    __builtin_write_OSCCONL(OSCCON | 0x01);
    while (OSCCONbits.COSC != 0b001);
    while (OSCCONbits.LOCK != 1);
}

void system_GPIO_init() {
    // All AD Converter OFF
    ANSELA = 0x0000;
    ANSELB = 0x0000;
    ANSELC = 0x0000;
    ANSELE = 0x0000;
    
    // init pin directions
    
    SPI_1_SCK_TRIS                  = OUTPUT_PIN;
    SPI_1_SDO_TRIS                  = OUTPUT_PIN;
    SPI_1_SDI_TRIS                  = INPUT_PIN;

    CS_TOUCH_TRIS                   = OUTPUT_PIN;
    CS_IO_EXP_16_1_2_TRIS           = OUTPUT_PIN;
    
    
    
    SPI_2_SCK_TRIS                  = OUTPUT_PIN;
    SPI_2_SDO_TRIS                  = OUTPUT_PIN;
    SPI_2_SDI_TRIS                  = INPUT_PIN;
    
    SPI_2_CS_PT100A_TRIS            = OUTPUT_PIN;
    SPI_2_CS_PT100B_TRIS            = OUTPUT_PIN;
    SPI_2_CS_AD_TRIS                = OUTPUT_PIN;
    SPI_2_CS_DA_TRIS                = OUTPUT_PIN;
    
    
    
    SPI_3_SCK_TRIS                  = OUTPUT_PIN;
    SPI_3_SDO_TRIS                  = OUTPUT_PIN;
    SPI_3_SDI_TRIS                  = INPUT_PIN;
    
    CS_1_SLAVE_TRIS                 = OUTPUT_PIN;
    CS_2_SLAVE_TRIS                 = OUTPUT_PIN;
    CS_3_SLAVE_TRIS                 = OUTPUT_PIN;
    CS_4_SLAVE_TRIS                 = OUTPUT_PIN;
    
    
    
    
    TOUCH_BUSY_TRIS                 = INPUT_PIN;
    
    I2C_CLK_TRIS                    = OUTPUT_PIN;
    I2C_DATA_TRIS                   = OUTPUT_PIN;
    
    I2C_WP_TRIS                     = OUTPUT_PIN;
    
    
    
//    DISP_LIGHT_TRIS                 = OUTPUT_PIN;
    DISP_DATA0_TRIS                 = OUTPUT_PIN;
    DISP_DATA1_TRIS                 = OUTPUT_PIN;
    DISP_DATA2_TRIS                 = OUTPUT_PIN;
    DISP_DATA3_TRIS                 = OUTPUT_PIN;
    DISP_DATA4_TRIS                 = OUTPUT_PIN;
    DISP_DATA5_TRIS                 = OUTPUT_PIN;
    DISP_DATA6_TRIS                 = OUTPUT_PIN;
    DISP_DATA7_TRIS                 = OUTPUT_PIN;
    
    DISP_CE_TRIS                    = OUTPUT_PIN;
    DISP_RESET_TRIS                 = OUTPUT_PIN;
    DISP_WRITE_TRIS                 = OUTPUT_PIN;
    DISP_READ_TRIS                  = OUTPUT_PIN;
    DISP_CD_TRIS                    = OUTPUT_PIN;
    
    INT0_PIN_TRIS                   = INPUT_PIN;
//    EXT_RESET_TRIS                  = OUTPUT_PIN; //INPUT_PIN;
    
    IN_11_TRIS                      = INPUT_PIN;
//    IN_02_TRIS                      = INPUT_PIN;
//    IN_03_TRIS                      = INPUT_PIN;
//    IN_04_TRIS                      = INPUT_PIN;
    IN_07_TRIS                      = INPUT_PIN;
    IN_06_TRIS                      = INPUT_PIN;
//    IN_07_TRIS                      = INPUT_PIN;
//    IN_08_TRIS                      = INPUT_PIN;
//    IN_09_TRIS                      = INPUT_PIN;
//    IN_10_TRIS                      = INPUT_PIN;
//    IN_11_TRIS                      = INPUT_PIN;
    
    
    
//    BUZZER_TRIS                     = OUTPUT_PIN;
    LED_RUN_TRIS                    = OUTPUT_PIN;
    
    
    
    OUT_01_TRIS                     = OUTPUT_PIN;
//    OUT_02_TRIS                     = OUTPUT_PIN;
//    OUT_03_TRIS                     = OUTPUT_PIN;
//    OUT_04_TRIS                     = OUTPUT_PIN;
//    OUT_05_TRIS                     = OUTPUT_PIN;
    OUT_06_TRIS                     = OUTPUT_PIN;
//    OUT_07_TRIS                     = OUTPUT_PIN;
//    OUT_08_TRIS                     = OUTPUT_PIN;
//    OUT_09_TRIS                     = OUTPUT_PIN;
//    OUT_10_TRIS                     = OUTPUT_PIN;
//    OUT_11_TRIS                     = OUTPUT_PIN;
    
    OUT_12_TRIS                     = OUTPUT_PIN;   // PWM 1
    OUT_13_TRIS                     = OUTPUT_PIN;   // PWM 2
    
    
    
    //reset outputs
    SPI_2_CS_PT100A                       = 1;    // INV
    SPI_2_CS_PT100B                       = 1;    // INV
    CS_1_SLAVE                      = 0;    // INV
    
    CS_TOUCH                        = 1;
//  CS_IO_EXP_16_1_2_TRIS           = 1;
    SPI_1_SDO                       = 0;    // DIR TOUCH - INV ALTRI
    SPI_1_SCK                       = 0;    // DIR TOUCH - INV ALTRI
    
    I2C_CLK                         = 1;
I2C_DATA_OUT                    = 1;
    I2C_WP                          = 1;
    
//    BUZZER                          = 0;
    
    
    
    DISP_DATA0                      = 0;
    DISP_DATA1                      = 0;
    DISP_DATA2                      = 0;
    DISP_DATA3                      = 0;
    DISP_DATA4                      = 0;
    DISP_DATA5                      = 0;
    DISP_DATA6                      = 0;
    DISP_DATA7                      = 0;
    
    DISP_CE                         = 1;
    DISP_RESET                      = 1;
    DISP_WRITE                      = 1;
    DISP_READ                       = 1;
    DISP_CD                         = 0;
    
//    DISP_LIGHT                      = 0;
}
