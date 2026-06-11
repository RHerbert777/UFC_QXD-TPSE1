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
// Offsets do Timer 
#define DMTIMER_TCLR   (0x24)
#define DMTIMER_TCRR   (0x28)
#define DMTIMER_TLDR   (0x2C)
#define DMTIMER_TWPS   (0x48)
#define DMTIMER_TSICR  (0x40)

// Máscaras de Bits
#define DMTIMER_TCLR_ST       (1 << 0)  // Bit 0 liga/desliga o Timer
#define DMTIMER_TSICR_POSTED  (1 << 2)  // Bit 2 habilita o modo Posted

// Flags de status de gravação pendente (TWPS)
#define DMTIMER_TWPS_W_PEND_TCLR (1 << 0)
#define DMTIMER_TWPS_W_PEND_TCRR (1 << 1)

// Macros de verificação do Posted Mode
#define DMTIMER_WRITE_POST_TCLR  DMTIMER_TWPS_W_PEND_TCLR
#define DMTIMER_WRITE_POST_TCRR  DMTIMER_TWPS_W_PEND_TCRR

void disable_wdt(void);

void timerSetup(void);
// Trava a CPU pelo tempo em milissegundos
void mtimerDelay(uint32_t ms);

void stimerDelay();

#endif
