#include <xc.h>
#define _XTAL_FREQ 4000000

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pic16f887.h>

#define RS PORTDbits.RD6
#define EN  PORTDbits.RD7
#define TRISRS  TRISDbits.TRISD6
#define TRISEN  TRISDbits.TRISD7
#define PUERTOTRS  TRISB
#define PUERTO PORTB

void lcd_init();
void lcd_control(char);
void lcd_dato(uint8_t);
void lcd_clear_display();
void lcd_cursor_home();
void lcd_print(char*);
void lcd_print_with_position(int, int, char*);
void lcd_goto(char, char);
void lcd_put_caracter(char, char[]);
void lcd_time_control();

void lcd_init(){
    TRISRS = 0; //configuro el RS como salida
    TRISEN = 0; //configuro el pin E como salida
    PUERTOTRS = 0b00000000;//condiguro el nibble mas significativo como salida
    RS = 0; //rs en 0
    EN = 0; //E en 0
    PUERTO = 0; //en 0
    
    __delay_ms(50);//tiempo de espera de puertos
    lcd_control(0x02);// Cursor al inicio
    lcd_control(0x38);//Configuracion a 8 bits, 2 lineas y fuente de 5*8(function set)
    lcd_control(0x0C);//LCD encendido, Cursor apagado
    lcd_control(0x06);//no rota el mensaje y se incrementa el contador de dirección
    
}

void lcd_control(char dato){
    RS = 0;//por que estoy mandando instrucciones
    PUERTO = dato; //ya que estoy enviando los 8 bits 
    EN = 1;
    __delay_us(5);
    EN = 0;
    __delay_us(5);
    __delay_ms(2);
}

void lcd_dato(uint8_t dato){
    RS = 1;//estoy mandando datos
    PUERTO = dato;
    EN = 1;
    __delay_us(5);
    EN = 0;
    __delay_us(5);
    __delay_us(50);
}

void lcd_clear_display(){
    lcd_control(0x01);
    __delay_ms(2);
}
void lcd_cursor_home(){
    lcd_control(0x02);
    __delay_ms(2);
}
void lcd_print(char *dato){
    while(*dato){
        lcd_dato(*dato);
        dato++;
    }
}
void lcd_print_with_position(int x, int y, char *dato){
    char posicion;
    switch(y){
        case 1: posicion = 0x80 + x;
        break;
        case 2: posicion = 0xC0 + x;
        break;
        default: posicion = 0x80 + x;
        break;
      
    }
    lcd_control(posicion);
    lcd_print(dato);
}
void lcd_goto(char x, char y){
    char posicion;
    switch(y){
        case 1:  posicion = 0x80 + x;
        break;
        case 2:  posicion = 0xC0 + x;
        break;
        default: posicion = 0x80 + x;
        break;   
    }
    lcd_control(posicion);
}
void lcd_put_caracter(char adress, char caracter[]){
    int i;
    lcd_control(0x40 + (adress * 8));
    for(i = 0; i<8; i++){
        lcd_dato(caracter[i]);
    }
}
