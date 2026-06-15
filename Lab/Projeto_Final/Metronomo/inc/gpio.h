#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

//whach dog timmer
#define SOC_WDT_1_REGS (0x44E35000)
#define WDT_WSPR       (0x48)
#define WDT_WWPS       (0x34)

//CLOCK 
#define CKM_PER_GPIO1_CLKCTRL  (0x0AC)
#define CKM_PER_GPIO2_CLKCTRL  (0xB0)

//Modulo de controle
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
#define GPIO_DATAOUT           (0x13C)
#define GPIO_OUTPUT            (0x000)
//Controle
#define GPIO_SETDATAOUT        (0x194)
#define GPIO_CLEANDATAOUT      (0X190)

//Seta o CLock do modulo em PRCM
void AT_MOD(unsigned int MODULO);

//Recebe o pino e a direção entrada e ou saida e a base do gpio que vamos setar
void CONF_DIR(unsigned int GPIO_BASE, uint32_t pin, uint32_t dir);

//Seta o pin como 1 (alto)
void GPIO_SET_PIN(unsigned int GPIO_BASE, uint32_t pin);

//Seta o pin como 0 (baixo)
void GPIO_CLEAN_PIN(unsigned int GPIO_BASE, uint32_t pin);

//Seta o modulo debouce como ANABLE que garante o funcionamento do nosso botão
void GPIO_DEBOUNCE_ENABLE(unsigned int MODULO);

//Seta a multiplexação do pino, recebe MODE = 0-7 e Modulo a ser setado
void MUX_CONFIG(uint32_t CONFIG, unsigned int MODULO);

uint32_t GPIO_READ_PIN(unsigned int GPIO_BASE, uint32_t pin) ;
#endif