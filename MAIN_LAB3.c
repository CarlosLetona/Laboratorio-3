/*la libreria de lcd la hice con ayuda de esto links:
 * https://electrosome.com/lcd-pic-mplab-xc8/
 * https://forum.arduino.cc/index.php?topic=502468.0
 * https://deepbluembedded.com/interfacing-16x2-lcd-with-pic-microcontrollers-mplab-xc8/
 * https://social.msdn.microsoft.com/Forums/es-ES/52c33e42-da42-405a-8523-0183c8be945e/convertir-en-binario-a-hex-y-decimal?forum=vcses
 * y principalmente:
 * https://www.youtube.com/watch?v=RgTMbQ6lssI
 * Del video me base, haciendo los cambios respectivos para lograr que la lcd funcione a 8 bits
 * Para mandar los valores de las variables a la lcd use estos links
 * https://www.geeksforgeeks.org/sprintf-in-c/
 * https://electrosome.com/lcd-pic-mplab-xc8/
 */

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
#include "LCD.h"

void configuracion(void);
float conversion(uint8_t valor_pot);
char s[50]; 

uint8_t valor_recibido;
uint8_t bandera = 1;
uint8_t turno = 1;
uint8_t contador = 10;

uint8_t potenciometro_1 = 0b01010101;//RE0(AN5)habilito el adc
uint8_t potenciometro_2 = 0b01011001;// RE1(AN6) 
uint8_t valor_pot1 = 0;
uint8_t valor_pot2 = 0;
float valor1_f;
float valor2_f;
float a;

void main(void) {
    configuracion();
    config_transimicion();
    config_recepcion();
    lcd_init();
    lcd_cursor_home();
    lcd_clear_display();
    lcd_print("V1    V2    Cont");
    config_adc();
    while(1){
        bandera = turnos_adc(bandera,potenciometro_1,potenciometro_2,turno);
        transmicion(valor_pot1, valor_pot2);
        valor1_f = conversion(valor_pot1);
        valor2_f = conversion(valor_pot2);
        sprintf(s, "%3.2f", valor1_f);
        lcd_goto(0, 2);//posicion 0 en x y 2 fila
        lcd_print(s);
        sprintf(s, "%3.2f", valor2_f);
        lcd_goto(6, 2);//posicion 6 en x y 2 fila
        lcd_print(s);
        sprintf(s, "%d", contador);
        lcd_goto(13, 2);//posicion 14 en x y 2 fila
        lcd_print(s);
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
    }if(PIR1bits.ADIF == 1){
        if(turno == 1){
            bandera = 1; 
            turno = 2;
            valor_pot1 = ADRESH;
            ADRESH = 0;
  
        }else if(turno == 2){
            bandera = 1;
            turno = 1;
            valor_pot2 = ADRESH;
            ADRESH = 0;
            }
    PIR1bits.ADIF = 0; //limpio la bandera
  }  
}

void configuracion(void){
    TRISD = 0;
    PORTD = 0;
    TRISC = 0b10000000; //Rx como entrada, y tx como salida
    PORTC = 0;
    TRISE =  0b00000011;//RE0 y RE1 entrada
    ANSEL =  0b01100000;//AN5 como analógico
}
float conversion(uint8_t valor_pot){
     a = 0.0196 * (float)valor_pot;
    return (a);
}
