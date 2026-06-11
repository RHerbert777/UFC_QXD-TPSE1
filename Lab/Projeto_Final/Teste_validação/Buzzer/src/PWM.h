#ifndef PWM_H
#define PWM_H
#include <stdint.h>

//whach dog timmer DESABILITADO
#define SOC_WDT_1_REGS (0x44E35000)
#define WDT_WSPR       (0x48)
#define WDT_WWPS       (0x34)

#define HWREG(x) (*((volatile unsigned int *)(x)))
#define HWREG_16(x) (*((volatile unsigned short *)(x)))

//CLOCK 
#define SOC_PRCM_REGS          (0x44E00000) //PRCM responsavel por mandar energia ao pino
#define SOC_CM_PER_REGS        (SOC_PRCM_REGS + 0)

//Direção ------------------------------------------------------------
#define CM_CONF_GPMC_A2 (0x848)
#define SOC_CONTROL_REGS (0x44E10000)

//clock global
#define PWMSS_CTRL (0x664)

//PWM inicialização --------------------------------------------------
#define SOC_PWMSS1_REGS  0x48302000
#define CM_PER_EPWMSS1_CLKCTRL  (0x0CC) //ofset para iniciar o PCRM

// 0x16 = No-Standby | No-Idle | Free Emulation] 010110
#define PWMSS1_SYSCONFIG    (0x04) //local do sisconfig

#define PWMSS1_CLKCONFIG    (0x08)

#define PWMSS1_CLKSTATUS   (0x0C)

// DEFINIÇÕES DO EHRPWM1
#define SOC_EHRPWM1_REGS (0x48302200)
#define TBCTL            (0x00) 
#define TBPRD            (0x0A) 
#define CMPA             (0x12) 
#define AQCTLA           (0x16)
#define PWM_CLOCK_HZ     (0x5F5E100) 



//Usa procesamente para delay
void delay(volatile unsigned int tempo);

void disable_wdt(void);

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