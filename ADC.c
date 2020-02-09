#include <stdint.h>
#include <pic16f887.h>
#include "ADC_H.h"

#define _XTAL_FREQ 4000000

void config_adc(void){
    PIR1bits.ADIF = 0;//estara en 1 cuando la conversión se complete
    PIE1bits.ADIE = 1;//habilito la interrupcion de adc
    ADCON1bits.ADFM = 0; //justificado a la izquierda
    INTCON = 0b11101000;//GIE,PEIE, T0IE, RBIE ACTIVAS
}

uint8_t turnos_adc(uint8_t bandera, uint8_t pot_1,uint8_t pot_2,uint8_t turno){
    if(bandera == 1 && turno == 1){
        ADCON0 = pot_1;
        __delay_us(25);
        ADCON0bits.GO = 1;
        }else if(bandera == 1 && turno == 2){
        ADCON0 = pot_2;
        __delay_us(25);
        ADCON0bits.GO = 1;
        }
        return(0);
   }
