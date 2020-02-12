#include <stdint.h>
#include <xc.h>
#include <pic16f887.h>
#define _XTAL_FREQ 4000000

void config_transimicion(void){
    TXSTAbits.SYNC = 0;//modo asincrono
    TXSTAbits.TXEN = 1;//habilito la comunicacion
    //baudrate a 9600
    TXSTAbits.BRGH = 0;//para el baudrate
    BAUDCTLbits.BRG16 = 1;
    SPBRG = 25;
    RCSTAbits.SPEN = 1;//habilito el tx como salida
}
void transmicion (char *valor1){ //citado de diego equite
    INTCONbits.GIE = 1;
    TXREG = valor1[0];
    while(TRMT == 0){}
    TXREG = valor1[1];
    while(TRMT == 0){}
    TXREG = valor1[2];
    while(TRMT == 0){}
    TXREG = valor1[3];
    while(TRMT == 0){}
    TXREG = 0x20;
    while(TRMT == 0){}
}
