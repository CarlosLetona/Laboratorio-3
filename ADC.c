#include <stdint.h>
#include <pic16f887.h>
#include "ADC_H.h"

#define _XTAL_FREQ 4000000

void config_adc(uint8_t bandera){
    INTCON	 = 0b11101000;//GIE,PEIE, T0IE, RBIE ACTIVAS
    PIR1bits.ADIF = 0;//estara en 1 cuando la conversión se complete
    PIE1bits.ADIE = 1;//habilito la interrupcion de adc
    ADCON1bits.ADFM = 0; //justificado a la izquierda

    if(bandera == 1){
        __delay_us(10);
        bandera = 0;
        ADCON0bits.GO = 1;
        
        }
   }
