#include <stdint.h>
#include <pic16f887.h>


void config_recepcion(void){
    TXSTAbits.SYNC = 0;//modo asincrono
    TXSTAbits.TXEN = 1;//habilito la comunicacion
    //baudrate a 9600
    TXSTAbits.BRGH = 0;//para el baudrate
    BAUDCTLbits.BRG16 = 1;
    SPBRG = 25;
    RCSTAbits.SPEN = 1;//habilito el modulo
    RCSTAbits.CREN = 1;//habilito el receptor
    RCSTAbits.FERR = 0;
    RCSTAbits.OERR = 0;
    PIE1bits.RCIE = 1;
        
}

