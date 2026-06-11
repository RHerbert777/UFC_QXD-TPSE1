#ifndef UART_H
#define UART_H
#include <stdint.h>
#define SOC_WDT_1_REGS (0x44E35000)
#define WDT_WSPR       (0x48)
#define WDT_WWPS       (0x34)

//ofser para acordar o modulo PRCM
#define CM_WKUP_UART0_CLKCTRL 0xB4

//Setup
void uartSetup(void);

//Saida
void uartPutChar(char c);
void uartPutString(char *str);

//entrada
uint8_t uartAvailable(void); //flag que verifica teclado
char uartGetChar(void);

#endif