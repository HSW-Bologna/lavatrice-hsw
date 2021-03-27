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

