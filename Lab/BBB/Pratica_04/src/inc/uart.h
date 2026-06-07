#ifndef UART_H
#define UART_H

#define SOC_PRCM_REGS          (0x44E00000) //PRCM responsavel por mandar energia ao pino
#define SOC_CM_PER_REGS        (SOC_PRCM_REGS + 0)

#define SOC_WDT_1_REGS (0x44E35000)
#define WDT_WSPR       (0x48)
#define WDT_WWPS       (0x34)
//ofser para acordar o modulo PRCM
#define CM_WKUP_UART0_CLKCTRL 0xB4

#endif