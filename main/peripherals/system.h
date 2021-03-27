/******************************************************************************/
/*                                                                            */
/*  HSW snc - Casalecchio di Reno (BO) ITALY                                  */
/*  ----------------------------------------                                  */
/*                                                                            */
/*  modulo: system.h                                                          */
/*                                                                            */
/*      inizializzazione REGISTRI micro per START                             */
/*                                                                            */
/*  Autore: Maldus(Mattia MALDINI) & Massimo ZANNA                            */
/*                                                                            */
/*  Data  : 05/12/2019      REV  : 00.0                                       */
/*                                                                            */
/*  U.mod.: 29/02/2020      REV  : 01.0                                       */
/*                                                                            */
/******************************************************************************/

#ifndef SYSTEM_H_INCLUDED
#define SYSTEM_H_INCLUDED

#include <xc.h>
#define FOSC 120000000 //TODO: impostare questa macro piu' precisamente 120223125
#define FCY (FOSC/2)
#include <libpic30.h>

void system_oscillator_config(void);

#endif

