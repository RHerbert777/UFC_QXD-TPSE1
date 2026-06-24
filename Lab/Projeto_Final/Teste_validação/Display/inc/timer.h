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

// Offsets internos
#define DMTIMER_IRQSTATUS      (0x28)
#define DMTIMER_IRQENABLE_SET  (0x2C)
#define DMTIMER_IRQENABLE_CLR  (0x30) //desabilita geração de interupção
#define DMTIMER_TCLR           (0x38)  // Registradores de controle
#define DMTIMER_TCRR           (0x3C)  // contador
#define DMTIMER_TLDR           (0x40)  // recarga
#define DMTIMER_TWPS           (0x48)  // status
#define DMTIMER_TSICR          (0x54)  //controle modo posted

void disable_wdt(void);

void timer_PRCM_Setup(void);
// Trava a CPU pelo tempo em milissegundos
void mtimerDelay(uint32_t ms);

#endif
