#ifndef FUNC_H
#define FUNC_H
#include "BBB_REGS.h"
#include <stdint.h>

static void delay() {
    volatile unsigned int ra;
    for (ra = 0; ra < 1000000; ra++);
}

//Desabilita o whach dog timmer 
void disable_wdt(void);

//Recebe o pino e a direção entrada e saida
void CONF_DIR(uint32_t pin, uint32_t dir);

//Seta o pin como 1 (alto)
void GPIO1_SET_PIN(uint32_t pin);

//Seta o pin como 0 (baixo)
void GPIO1_CLEAN_PIN(uint32_t pin);

#endif