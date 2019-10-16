/* 
 * File:   main.c
 * Author: tpeea
 *
 * Created on 16 octobre 2019, 16:50
 */

// DSPIC30F6014 Configuration Bit Settings

// 'C' source line config statements

#include <xc.h>

// FOSC
#pragma config FPR = XT_PLL16           // Primary Oscillator Mode (XT w/PLL 16x)
#pragma config FOS = PRI                // Oscillator Source (Primary Oscillator)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Enabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = NONE            // Brown Out Voltage (Reserved)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config MCLRE = MCLR_EN          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * 
 */


void INIT(void);
void LCDinit(void);
void LCDreset(void);
void LCDhome(void);
void LCDhomeclear(void);
void LCDwritechar(char);

void __attribute__((interrupt,auto_psv)) _INT1Interrupt(void)
{
PORTDbits.RD0=~PORTDbits.RD0;
IFS1bits.INT1IF=0; // acquittement FLAG
}
void __attribute__((interrupt,auto_psv)) _INT2Interrupt(void)
{
PORTDbits.RD1=~PORTDbits.RD1;
IFS1bits.INT2IF=0; // acquittement FLAG
}
void __attribute__((interrupt,auto_psv)) _INT3Interrupt(void)
{
PORTDbits.RD2=~PORTDbits.RD2;
_INT3IF=0; // acquittement FLAG
}
void __attribute__((interrupt,auto_psv)) _INT4Interrupt(void)
{
PORTDbits.RD3=~PORTDbits.RD3;
_INT4IF=0; // acquittement FLAG
}

int main() 
{
    INIT();
    LCDinit();
    LCDwritechar();
    
    while(1);
}

void INIT()
{
TRISDbits.TRISD0=0; // configuration LED1 en sortie
// initialisation INT1 sur SW1
INTCON2bits.INT1EP=1; // interruption sur front descendant.
IPC4bits.INT1IP=1; // priorit´e 1
IFS1bits.INT1IF=0; // raz FLAG
IEC1bits.INT1IE=1; // autorisation interruption INT1

TRISDbits.TRISD1=0; // configuration LED2 en sortie
// initialisation INT2 sur SW1
INTCON2bits.INT2EP=1; // interruption sur front descendant.
_INT2IP=1; // priorit´e 1
IFS1bits.INT2IF=0; // raz FLAG
IEC1bits.INT2IE=1; // autorisation interruption INT2

TRISDbits.TRISD2=0; // configuration LED3 en sortie
// initialisation INT3 sur SW1
INTCON2bits.INT3EP=1; // interruption sur front descendant.
_INT3IP=1; // priorit´e 1
_INT3IF=0; // raz FLAG
_INT3IE=1; // autorisation interruption INT3

TRISDbits.TRISD3=0; // configuration LED4 en sortie
// initialisation INT4 sur SW1
INTCON2bits.INT4EP=1; // interruption sur front descendant.
_INT4IP=1; // priorit´e 1
_INT4IF=0; // raz FLAG
_INT4IE=1; // autorisation interruption INT4
}

void LCDinit()
{
    SPI2CONbits.PPRE=1;
    SPI2CONbits.SPRE=7;
    SPI2CONbits.CKE=0;
    SPI2CONbits.CKP=0;
    SPI2CONbits.MSTEN=1;
    TRISGbits.TRISG9=0;
    _LATG9=0;
    _SPIEN=1;
}
//TBF=0: données transmises
void LCDreset()
{
    SPI2BUF=0x80;
    while(_SPITBF);
}
void LCDhome()
{  
    SPI2BUF=0x81;
    while(_SPITBF);
}
void LCDhomeclear()
{
    SPI2BUF=0x82;
    while(_SPITBF);
}
void LCDwritechar(char c)
{
    SPI2BUF=0xA8;
    SPI2BUF=c;
    while(_SPITBF);
}