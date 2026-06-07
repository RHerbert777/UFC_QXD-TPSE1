#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

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
#define GPIO_DATAOUT           (0x13C)

//SOC_GPIO
#define SOC_GPIO_1_REGS        (0x4804C000)
#define SOC_GPIO_2_REGS        (0x481AC000)

//Controle
#define GPIO_SETDATAOUT        (0x194)
#define GPIO_CLEANDATAOUT      (0X190)

#define HWREG(x) (*((volatile unsigned int *)(x)))

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

//Algumas sequencias para teste
void SEQUENCIA_0_0 ();

void SEQUENCIA_0_1 ();

void SEQUENCIA_0_2 ();

void SEQUENCIA_1_0 ();

void SEQUENCIA_1_1 ();

void SEQUENCIA_1_2 ();
#endif