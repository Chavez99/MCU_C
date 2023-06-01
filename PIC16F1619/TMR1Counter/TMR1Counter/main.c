#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000
#include "flex_lcd.h"
#include "mcc_generated_files/mcc.h"
// PIC16F1619 Configuration Bit Settings
// CONFIG1
#pragma config FOSC = INTOSC // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config PWRTE = ON // Power-up Timer Enable (PWRT enabled)
#pragma config MCLRE = OFF // MCLR Pin Function Select (MCLR/VPP pin function is digital input)
#pragma config PLLEN = OFF // PLL Enable Bit (4x PLL is enabled when software sets the SPLLEN bit)
#pragma config WDTE = OFF // Watchdog Timer Enable (WDT disabled)
//VARIABLES DEFINITION
unsigned char buffer1[20],buffer2[20];
int i;
long counter;
unsigned int TMR;
unsigned int TIMER;
//Initialize .017s Period Timer
void TMR0_Timer(void){
    T0CKIPPS = 0b00000010;
    OPTION_REG = 0b00100111; // PSA assigned; PS 1:256;
    TMR0 = 225;// TMR0 Dec125.79; From High to Low
    INTCONbits.TMR0IF = 0;// Clear Interrupt flag before enabling the interrupt
    INTCONbits.TMR0IE = 1;// Enabling TMR0 interrupt
}

void TMR1_Counter(void){
    T1CON = 0b10000100;
}

void __interrupt() ISR_TIMER0(void){
    if(INTCONbits.TMR0IF ==1){
        LATBbits.LATB4 = ! LATBbits.LATB4;
        counter = TMR1;
        TMR0 = 0x7D;//Restart TMR Reg
        TMR1 = 0;
        INTCONbits.TMR0IF = 0;
    }
}


void main(void) {   
    SYSTEM_Initialize();
    //OSCILLATOR AT 4MHz
    OSCCONbits.IRCF = 0b1101; //Fosc at 4MHz
    OSCCONbits.SCS = 0b00; //System clock select
    OSCTUNE = 0; //Running at factory cal frequency
    
    //PORT CONFIGURATION
    ANSELA = 0b00000000; //Set as digital
    ANSELB = 0b00000000; //Set as digital
    ANSELC = 0b00000000; //Set as digital
    TRISA = 0b00100100; //RA5 and RA2 set as digital input
    TRISB = 0b00000000; //Set as output
    TRISC = 0b00000000; //Set as output
    
    //Clear port registers
    PORTA = 0;
    PORTB = 0; 
    PORTC = 0; 
    
    //INT Config
    INTCONbits.GIE = 1; 
    INTCONbits.T0IF = 0; 
    INTCONbits.TMR0IE = 1; 
    TMR0_Timer();
    TMR1_Counter();
    
    


    //LCD LIBRARY CONFIGURATION
    //Configure the library to define pins being dedicated to the LCD.
    //LCD CONFFIGURATION
    Lcd_Init();
    Lcd_Cmd(LCD_CLEAR); //Clear the LCD
    Lcd_Cmd(LCD_CURSOR_OFF); //Disable cursor visualization
    __delay_ms(100); //Wait.
    
    uint16_t DC;
    DC = 63;
    
    while(1){
        PWM3_LoadDutyValue(DC);
        TIMER = 0;
        TMR = 0;
        TMR1 = 0;
        TMR1L = 0;
        TMR1H = 0;
        T1CONbits.TMR1ON = 1;
        __delay_ms(1000); //Wait.
        TMR = TMR1H;
        TMR = TMR<<8;
        TMR = TMR + TMR1L;
        T1CONbits.TMR1ON = 0;
        TMR1 = 0;
        TMR1L = 0;
        TMR1H = 0;
        TIMER = TMR;//Change to int variable so as to show on LCD
        //ADRESH content is shifted 8 bits to the right, but can also be multiplied by 255d
        sprintf((char *)buffer1,"Cuenta: %5d", TIMER);
        sprintf((char *)buffer2,"H:%2d L:%2d", TMR1H, TMR1L);
        Lcd_Out2(1,1,(char *)buffer1);
        Lcd_Out2(2,1,(char *)buffer2);
        __delay_ms(1000); //Wait.
    }          
 return;
}
