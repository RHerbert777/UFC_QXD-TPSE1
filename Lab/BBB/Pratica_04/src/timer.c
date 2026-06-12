#include "timer.h"

#define CKM_PER_TIMER7_CLKCTRL 0x7C

// Endereços absolutos específicos do Timer 7
#define PRCM_TIMER7_CLKCTRL  (0x44E00000 + 0x7C)
#define DPLL_TIMER7_CLKSEL   (0x44E00500 + 0x04)

// Offsets internos
#define DMTIMER_TCLR   0x38  // Registradores de controle
#define DMTIMER_TCRR   0x3C  // contador
#define DMTIMER_TLDR   0x40  // recarga
#define DMTIMER_TWPS   0x48  // status
#define DMTIMER_TSICR  0x54  //controle modo posted

//Desabilita o whach dog timmer 
void disable_wdt(void) {
    unsigned int addr_wspr = SOC_WDT_1_REGS + WDT_WSPR;
    unsigned int addr_wwps = SOC_WDT_1_REGS + WDT_WWPS;

    HWREG(addr_wspr) = 0xAAAA;
    while ( (HWREG(addr_wwps) & (1 << 4)) != 0 );

    HWREG(addr_wspr) = 0x5555;
    while ( (HWREG(addr_wwps) & (1 << 4)) != 0 );
}

void timerSetup(void) {
    //PRCM
    HWREG(PRCM_TIMER7_CLKCTRL) |= 0x02; 
    while ((HWREG(PRCM_TIMER7_CLKCTRL) & 0x03) != 0x02);

    HWREG(DPLL_TIMER7_CLKSEL) = 0x01; //conecta ao cristal de 24mhz

    //posted libera a cpu para executar outras tarefas
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TSICR) |= (1 << 2);

    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 0));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCLR) &= ~(1 << 0);

    //zera o contador
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 1));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCRR) = 0;
    
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 2));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TLDR) = 0;
}

void mtimerDelay(uint32_t ms) {
    // 1 milissegundo = 24.000 ticks a 24 MHz
    uint32_t ticks_needed = ms * 24000;
    
    // Zera o contador
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 1));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCRR) = 0;
    
    // Dá o Start
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 0));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCLR) |= (1 << 0);
    
    // Fica lendo até atingir o tempo
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCRR) < ticks_needed);
    
    // Dá o Stop
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 0));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCLR) &= ~(1 << 0);
}
//Uma ideia interesante é implementar timeout em codigos que podem entrar em loop um temp com um numero grande ele
//decrementando toda vez que entrar se ele decrementar muito e o codigo não tiver sido executado significa erro demorou
//muito tempo em no while retorna um erro 