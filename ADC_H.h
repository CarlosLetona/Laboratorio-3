
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __ADC_H_
#define	__ADC_H_

#include <xc.h> // include processor files - each processor file is guarded.  
#include <stdint.h>

uint8_t turnos_adc(uint8_t bandera, uint8_t pot_1,uint8_t pot2,uint8_t turno);
void config_adc(void);

#endif

