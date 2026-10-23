// FASE 3 - Paro de emergencia: ISR en IO33 con debounce por ciclos (CCOUNT)
#pragma once
#include <stdint.h>

void     estopInit();        // INPUT_PULLUP + interrupcion FALLING en IO33
bool     estopTakeFlag();    // lee y limpia la bandera con spinlock (solo desde loop())
uint32_t estopCount();       // pulsaciones validas acumuladas
bool     estopReleased();    // true si el microinterruptor esta suelto (pin en HIGH)