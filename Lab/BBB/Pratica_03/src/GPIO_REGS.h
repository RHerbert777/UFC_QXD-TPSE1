#ifndef _GPIO_REGS_H_
#define _GPIO_REGS_H_

//whach dog timmer
#define SOC_WDT_1_REGS (0x44E35000)
#define WDT_WSPR       (0x48)
#define WDT_WWPS       (0x34)

//CLOCK 
#define SOC_PRCM_REGS          (0x44E00000) //PRCM responsavel por mandar energia ao pino
#define CKM_PER_GPIO1_CLKCTRL  (0x0AC)
#define CKM_PER_GPIO2_CLKCTRL  (0xB0)
#define SOC_CM_PER_REGS        (SOC_PRCM_REGS + 0)

//Modulo de controle
#define SOC_CONTROL_REGS       (0x44E10000)
#define CM_CONF_GPMC_A5        (0x0854)
#define CM_CONF_GPMC_A6        (0x0858)
#define CM_CONF_GPMC_A7        (0x085C)
#define CM_CONF_GPMC_A8        (0x0860)
#define CM_CONF_GPMC_CLK       (0x088C)
#define CM_CONF_LCD_VSYNC      (0x08E0) 
#define CM_CONF_LCD_PCLK       (0x08E8)
//DIREÇÃO
#define GPIO_OE                (0x134)
#define GPIO_DATAIN            (0x138)
#define SOC_GPIO_1_REGS        (0x4804C000)
#define SOC_GPIO_2_REGS        (0x481AC000)

//Controle
#define GPIO_SETDATAOUT        (0x194)
#define GPIO_CLEANDATAOUT      (0X190)

#define HWREG(x) (*((volatile unsigned int *)(x)))

#endif

