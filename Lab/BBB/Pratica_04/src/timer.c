#include "timer.h"

#define reset 0x00000000
#define CKM_PER_TIMER7_CLKCTRL 0x7C

// Endereços absolutos específicos do Timer 7
#define PRCM_TIMER7_CLKCTRL  (0x44E00000 + 0x7C)
#define DPLL_TIMER7_CLKSEL   (0x44E00500 + 0x04)

// Offsets internos
//!o que cada um faz?
#define DMTIMER_TCLR   0x38  // Registradores de Controle
#define DMTIMER_TCRR   0x3C  // O verdadeiro Contador!
#define DMTIMER_TLDR   0x40  // Registrador de Recarga
#define DMTIMER_TWPS   0x48  // Status de gravação pendente
#define DMTIMER_TSICR  0x54
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
    //!Posivel alteraçao na nossa função prcm
    // 1. Liga o módulo no PRCM
    HWREG(PRCM_TIMER7_CLKCTRL) |= 0x02; 
    while ((HWREG(PRCM_TIMER7_CLKCTRL) & 0x03) != 0x02);

    // 2. Conecta a "Pilha" de 24MHz (Cristal externo)
    HWREG(DPLL_TIMER7_CLKSEL) = 0x01;

    // 3. Liga o Modo Posted para acesso rápido
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TSICR) |= (1 << 2);

    // 4. Para o timer por segurança
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 0));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCLR) &= ~(1 << 0);

    // 5. Zera os contadores
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 1));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCRR) = 0;
    
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 2));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TLDR) = 0;
}

void mtimerDelay(uint32_t ms) {
    // Matemática: 1 milissegundo = 24.000 ticks a 24 MHz
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
