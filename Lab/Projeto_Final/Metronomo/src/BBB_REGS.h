#ifndef _BBB_REGS_H_
#define _BBB_REGS_H_

//whach dog timmer
#define SOC_WDT_1_REGS (0x44E35000)
#define WDT_WSPR       (0x48)
#define WDT_WWPS       (0x34)
//CLOCK 
#define SOC_PRCM_REGS          (0x44E00000) //PRCM responsavel por mandar energia ao pino
#define CKM_PER_GPIO1_CLKCTRL  (0x0AC)
#define SOC_CM_PER_REGS        (SOC_PRCM_REGS + 0)
//Modulo de controle
#define SOC_CONTROL_REGS       (0x44E10000)
#define CM_CONF_GPMC_A5        (0x0854)
//DIREÇÃO
#define GPIO_OE                (0x134)
#define SOC_GPIO_1_REGS        (0x4804C000)
//Controle
#define GPIO_SETDATAOUT        (0x194)
#define GPIO_CLEANDATAOUT      (0X190)

#define HWREG(x) (*((volatile unsigned int *)(x)))

#endif

