#ifndef FUNC_H
#define FUNC_H
#include "BBB_REGS.h"
#include <stdint.h>

void disable_wdt(void);

static void delay() {
    volatile unsigned int ra;
    for (ra = 0; ra < 1000000; ra++);
}
//Ativa o modulo PCRM de um modulo com o offser colocando 2 enable, pode tambem ativar o gpio
void PRCM_ENABLE_MODE(uint32_t module_offset);

//Configura nosso buzzer 
void SET_BUZZER();

//Seta a frequencia do buzzer 
void PWM_SET_FREQUENCY(uint32_t freq_hz);

// pwm_play: Ativa o som (Duty Cycle = 50%)
void PWM_PLAY(void);

//pwm_stop: Silencia o som (Duty Cycle = 0%)
void PWM_STOP(void);

#endif