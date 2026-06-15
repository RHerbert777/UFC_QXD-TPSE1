#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>
#include "hw_types.h" 
#include "soc_AM335x.h"

#include <stdint.h>

//whach dog timmer
#define SOC_WDT_1_REGS (0x44E35000)
#define WDT_WSPR       (0x48)
#define WDT_WWPS       (0x34)

//modulo de pinos
#define CM_CONF_GPMC_A5        (0x0854)
#define CM_CONF_GPMC_A6        (0x0858)
#define CM_CONF_GPMC_A7        (0x085C)
#define CM_CONF_GPMC_A8        (0x0860)
#define CM_CONF_GPMC_CLK       (0x088C)
#define CM_CONF_LCD_VSYNC      (0x08E0) 
#define CM_CONF_LCD_PCLK       (0x08E8)
#define CM_PER_TIMER7_CLKCTRL  (0x44E0007C)

// Offsets internos
#define DMTIMER_TCLR   0x38  // Registradores de controle
#define DMTIMER_TCRR   0x3C  // contador
#define DMTIMER_TLDR   0x40  // recarga
#define DMTIMER_TWPS   0x48  // status
#define DMTIMER_TSICR  0x54  //controle modo posted


void disable_wdt(void);

void timerSetup(void);
// Trava a CPU pelo tempo em milissegundos
void mtimerDelay(uint32_t ms);

#endif
