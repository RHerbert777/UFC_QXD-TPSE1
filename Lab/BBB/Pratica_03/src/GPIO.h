#ifndef GPIO_H
#define GPIO_H
#include "GPIO_REGS.h"
#include <stdint.h>

static void delay() {
    volatile unsigned int ra;
    for (ra = 0; ra < 1000000; ra++);
}

//Desabilita o whach dog timmer 
void disable_wdt(void);

//Seta o CLock do modulo em PRCM
void AT_MOD(unsigned int MODULO);

//Recebe o pino e a direção entrada e ou saida e a base do gpio que vamos setar
void CONF_DIR(unsigned int GPIO_BASE, uint32_t pin, uint32_t dir);

//Seta o pin como 1 (alto)
void GPIO1_SET_PIN(unsigned int GPIO_BASE, uint32_t pin);

//Seta o pin como 0 (baixo)
void GPIO1_CLEAN_PIN(unsigned int GPIO_BASE, uint32_t pin);

//Seta o modulo debouce como ANABLE que garante o funcionamento do nosso botão
void GPIO_DEBOUNCE_ENABLE(unsigned int MODULO);

//Seta a multiplexação do pino, recebe MODE = 0-7 e Modulo a ser setado
void MUX_CONFIG(uint32_t CONFIG, unsigned int MODULO);

uint32_t GPIO_READ_PIN(unsigned int GPIO_BASE, uint32_t pin) ;

void SEQUENCIA_0_0 ();

void SEQUENCIA_0_1 ();

void SEQUENCIA_0_2 ();

void SEQUENCIA_1_0 ();

void SEQUENCIA_1_1 ();

void SEQUENCIA_1_2 ();
#endif