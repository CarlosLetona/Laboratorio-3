#pragma config FOSC = INTRC_CLKOUT// Oscillator Selection bits (INTOSC oscillator: CLKOUT function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 4000000

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f887.h>
#include "ADC_H.h"
#include "Transmicion.h"
#include "Recepcion.h"

void configuracion(void);

uint8_t valor_adc1 = 3;
uint8_t valor_adc2 = 5;
uint8_t valor_recibido;
uint8_t bandera = 1;
uint8_t contador = 0;

void main(void) {
    configuracion();
    config_transimicion();
    config_recepcion();
    while(1){
        config_adc(bandera);
        transmicion(valor_adc1, valor_adc2);
        PORTD = contador;
        if (valor_recibido == 0x2B){
            valor_recibido = 0;
            contador++;
        }else if(valor_recibido == 0x2D){
            valor_recibido = 0;
            contador--;
       }
    }
    return;
}
void __interrupt() ISR(void){
    if(PIR1bits.RCIF == 1){       
    if(RCSTAbits.OERR == 1){
        RCSTAbits.CREN = 0;
        __delay_us(255);
    }else{
        valor_recibido = RCREG;
    }
  }  
}

void configuracion(void){
    TRISD = 0;
    PORTD = 0;
    TRISC = 0b10000000; //Rx como entrada, y tx como salida
    PORTC = 0;
}