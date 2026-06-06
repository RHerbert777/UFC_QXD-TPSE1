#include "timer.h"

#define reset 0x00000000
#define CKM_PER_TIMER7_CLKCTRL 0x7C

void disable_wdt(void) {
    unsigned int addr_wspr = SOC_WDT_1_REGS + WDT_WSPR;
    unsigned int addr_wwps = SOC_WDT_1_REGS + WDT_WWPS;

    HWREG(addr_wspr) = 0xAAAA;
    while ( (HWREG(addr_wwps) & (1 << 4)) != 0 );

    HWREG(addr_wspr) = 0x5555;
    while ( (HWREG(addr_wwps) & (1 << 4)) != 0 );
}

//Ativa o Clock no PRCM do modulo
void AT_MOD_PRCM(unsigned int MODULO) {
    HWREG(SOC_CM_PER_REGS + MODULO) |= (0x2 << 0);
    
    while ((HWREG(SOC_CM_PER_REGS + MODULO) & (3 << 16)) != 0) {}
}
//retorna o status do registrador
static unsigned int DMTimerWritePostedStatusGet(unsigned int baseAdd) {
    /* Retorna o status do registrador TWPS */
    return (HWREG(baseAdd + DMTIMER_TWPS));
}

//Inicializa o timer da BBB apartir de sua base
void DMTimerInit(unsigned int baseAdd) {
    // 1. Configura a interface para o Posted Mode (reduz latência OCP)
    HWREG(baseAdd + DMTIMER_TSICR) |= DMTIMER_TSICR_POSTED;

    // 2. Garante que o timer está parado antes de configurar
    DMTimerDisable(baseAdd);

    // 3. Reseta o contador principal
    DMTimerCounterSet(baseAdd, reset);

    // 4. Reseta o registrador de recarga automática (TLDR)
    while (HWREG(baseAdd + DMTIMER_TWPS) & (1 << 2)); // Trava de segurança para o TLDR
    HWREG(baseAdd + DMTIMER_TLDR) = reset;
}

//Seta um valor para o contador do timer
void DMTimerCounterSet(unsigned int baseAdd, unsigned int counter) {
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCRR, baseAdd);
  
    HWREG(baseAdd + DMTIMER_TCRR) = counter;
}

//Ler o contador atual
unsigned int DMTimerCounterGet(unsigned int baseAdd) {
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCRR, baseAdd);
  
    return (HWREG(baseAdd + DMTIMER_TCRR));
}

//Inicializa o timer
void DMTimerEnable(unsigned int baseAdd) {
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCLR, baseAdd);
   
    HWREG(baseAdd + DMTIMER_TCLR) |= DMTIMER_TCLR_ST;
}

//Para o timer
void DMTimerDisable(unsigned int baseAdd) {
    DMTimerWaitForWrite(DMTIMER_WRITE_POST_TCLR, baseAdd);
    
    HWREG(baseAdd + DMTIMER_TCLR) &= ~DMTIMER_TCLR_ST;
}

void uDelay(unsigned int us) {
    // Calcula a quantidade total de "ticks" necessários (24 MHz = 24 ticks/us)
    unsigned int ticks_needed = us * TIMER_1US_COUNT;
    
    // Zera o contador
    DMTimerCounterSet(SOC_DMTIMER_7_REGS, 0);
    
    // Liga o cronômetro
    DMTimerEnable(SOC_DMTIMER_7_REGS);
    
    // TRAVA DA CPU: Fica lendo o contador até atingir o tempo desejado
    while (DMTimerCounterGet(SOC_DMTIMER_7_REGS) < ticks_needed);
    
    // Desliga o cronômetro (CORREÇÃO DA LINHA DO PDF)
    DMTimerDisable(SOC_DMTIMER_7_REGS); 
}