#include "soc_AM335x.h"
#include "uart.h"
#include "hw_types.h"

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

//! Apagar os comentarios e entender parte por parte do codigo
//! Lembre que precisa ativar o clock
void UART0_Init(void) {

    HWREG(SOC_CONTROL_REGS + 0x970) = (0x20 | 0); // RX
    HWREG(SOC_CONTROL_REGS + 0x974) = 0;          // TX

    HWREG(SOC_UART_0_REGS + 0x0C) = (1 << 7); // LCR = 0x80

    HWREG(SOC_UART_0_REGS + 0x00) = 0x1A; // DLL
    HWREG(SOC_UART_0_REGS + 0x04) = 0x00; // DLH

    HWREG(SOC_UART_0_REGS + 0x0C) = 0x03; 

    HWREG(SOC_UART_0_REGS + 0x20) = 0x00;
}