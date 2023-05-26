#include <xc.h>
#include <stdio.h>
#define _XTAL_FREQ 4000000
#include "flex_lcd.h"
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

//Initialize .017s Period Timer
void TMR0_temporizador(void){
    OPTION_REG = 0b00000110; // PSA assigned; PS 1:128;
    TMR0 = 0x7D;// TMR0 Dec125.79; From High to Low
    INTCONbits.TMR0IF = 0;// Clear Interrupt flag before enabling the interrupt
    INTCONbits.TMR0IE = 1;// Enabling TMR0 interrupt
}

void __interrupt() ISR_TIMER0(void){
    if(INTCONbits.TMR0IF ==1){
        LATBbits.LATB4 = ! LATBbits.LATB4;
        TMR0 = 0x7D;//Restart TMR Reg
        INTCONbits.TMR0IF = 0;
    }
}


void main(void) {   
    //OSCILLATOR AT 4MHz
    OSCCONbits.IRCF = 0b1101; //Fosc at 4MHz
    OSCCONbits.SCS = 0b00; //System clock select
    OSCTUNE = 0; //Running at factory cal frequency
    
    //PORT CONFIGURATION
    ANSELA = 0b0000000; //Set as digital
    ANSELB = 0b0000000; //Set as digital
    ANSELC = 0b0000000; //Set as digital
    TRISA = 0b0000000; //Set as output
    TRISB = 0b0000000; //Set as output
    TRISC = 0b0000000; //Set as output
    
    //Clear port registers
    PORTA = 0;
    PORTB = 0; 
    PORTC = 0; 
    
    //INT Config
    INTCONbits.GIE = 1; 
    INTCONbits.T0IF = 0; 
    INTCONbits.TMR0IE = 1; 
    TMR0_temporizador();
    
    


    //LCD LIBRARY CONFIGURATION
    //Configure the library to define pins being dedicated to the LCD.
    //LCD CONFFIGURATION
    Lcd_Init();
    Lcd_Cmd(LCD_CLEAR); //Clear the LCD
    Lcd_Cmd(LCD_CURSOR_OFF); //Disable cursor visualization
    __delay_ms(100); //Wait.
    
    
    while(1){
        //ADRESH content is shifted 8 bits to the right, but can also be multiplied by 255d
        sprintf((char *)buffer1,"Cuenta %03d", 5);
        sprintf((char *)buffer2,"DEC: %03d", 222);
        Lcd_Out2(1,1,(char *)buffer1);
        Lcd_Out2(2,1,(char *)buffer2);
        __delay_ms(100);


        for(i=10; i>0; i--){
            sprintf((char *)buffer1,"Cuenta %03d", i); 
            Lcd_Out2(1,1,(char *)buffer1);
            __delay_ms(100);
        }
        Lcd_Out2(2,1,"Testing LCD");
        __delay_ms(600);
        Lcd_Cmd(LCD_BLINK_CURSOR_ON);
        __delay_ms(1000);
        Lcd_Cmd(LCD_UNDERLINE_ON);
        __delay_ms(1000);
        Lcd_Cmd(LCD_CURSOR_OFF);
        __delay_ms(1000);
        Lcd_Cmd(LCD_CLEAR);
        __delay_ms(1000);
        Lcd_Out(1, 4, "Thanks");
        Lcd_Out(2, 4, "Thank");
        }          
 return;
}
