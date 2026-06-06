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

//CLOCK 
#define SOC_PRCM_REGS          (0x44E00000) //PRCM responsavel por mandar energia ao pino
#define SOC_CM_PER_REGS        (SOC_PRCM_REGS + 0)
#define CKM_PER_TIMER7_CLKCTRL (0x7C)

//Modulo de controle
#define SOC_CONTROL_REGS       (0x44E10000)

//modulo de pinos
#define CM_CONF_GPMC_A5        (0x0854)
#define CM_CONF_GPMC_A6        (0x0858)
#define CM_CONF_GPMC_A7        (0x085C)
#define CM_CONF_GPMC_A8        (0x0860)
#define CM_CONF_GPMC_CLK       (0x088C)
#define CM_CONF_LCD_VSYNC      (0x08E0) 
#define CM_CONF_LCD_PCLK       (0x08E8)

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

// Configuração do Clock (24 MHz)
#define TIMER_1US_COUNT 24

void disable_wdt(void);
void AT_MOD_PRCM(unsigned int MODULO);

static unsigned int DMTimerWritePostedStatusGet(unsigned int baseAdd);

void DMTimerCounterSet(unsigned int baseAdd, unsigned int counter);

unsigned int DMTimerCounterGet(unsigned int baseAdd);

void DMTimerEnable(unsigned int baseAdd);

void DMTimerDisable(unsigned int baseAdd);

void uDelay(unsigned int us);

// Macro profissional de barramento (Adaptada da pág. 6)
#define DMTimerWaitForWrite(reg, baseAdd) \
    if (HWREG(baseAdd + DMTIMER_TSICR) & DMTIMER_TSICR_POSTED) \
        while ((reg & DMTimerWritePostedStatusGet(baseAdd)))

#endif