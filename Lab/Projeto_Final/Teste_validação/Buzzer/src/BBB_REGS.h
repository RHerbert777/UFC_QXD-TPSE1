#ifndef _BBB_REGS_H_
#define _BBB_REGS_H_

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
#define SOC_EHRPWM1_REGS 0x48302200
#define TBCTL    0x00 
#define TBPRD    0x0A 
#define CMPA     0x12 
#define AQCTLA   0x16
#define PWM_CLOCK_HZ 0x5F5E100 //converte para exa em outro momento

#endif

