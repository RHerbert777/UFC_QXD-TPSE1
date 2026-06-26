#ifndef PWM_H
#define PWM_H
#include <stdint.h>
#include "hw_types.h"

//clock global
#define PWMSS_CTRL (0x664)

//PWM inicialização --------------------------------------------------
#define CM_PER_EPWMSS1_CLKCTRL  (0x0CC) //ofset para iniciar o PCRM

// 0x16 = No-Standby | No-Idle | Free Emulation] 010110
#define PWMSS1_SYSCONFIG    (0x04) //local do sisconfig
#define PWMSS1_CLKCONFIG    (0x08)
#define PWMSS1_CLKSTATUS    (0x0C)

// DEFINIÇÕES DO EHRPWM1
#define SOC_EHRPWM1_REGS (0x48302200)
#define TBCTL            (0x00) 
#define TBPRD            (0x0A) 
#define CMPA             (0x12) 
#define AQCTLA           (0x16)
#define PWM_CLOCK_HZ     (0x5F5E100) 

//Configura o BUZZER recebe o modulo do PRCM do buzzer
void SET_BUZZER(uint32_t module_offset);

//Seta a frequencia do buzzer 
void PWM_SET_FREQUENCY(uint32_t freq_hz);

// pwm_play: Ativa o som (Duty Cycle = 50%)
void PWM_PLAY(void);

//pwm_stop: Silencia o som (Duty Cycle = 0%)
void PWM_STOP(void);

#endif