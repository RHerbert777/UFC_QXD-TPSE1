#include "soc_AM335x.h"
#include "uart.h"
#include "hw_types.h"

// Endereços Absolutíssimos da UART0 e do Control Module (AM335x TRM)
#define CM_WKUP_UART0_CLKCTRL  (0x44E00400 + 0x00) // Endereço de energia da UART0
#define SOC_UART_0_REGS        0x44E09000          // Base da UART0

// Registradores do Control Module para Multiplexação (Pinos físicos da placa)
#define CONTROL_MODULE_BASE    0x44E10000
#define CONF_UART0_RXD         0x970
#define CONF_UART0_TXD         0x974

// Registradores internos da UART0
#define UART_RHR    (0x00)
#define UART_THR    (0x00)
#define UART_DLL    (0x00)
#define UART_DLH    (0x04)
#define UART_LCR    (0x0C)
#define UART_MDR1   (0x20)
#define UART_LSR    (0x14)

void uartSetup(void){
    //Ativar PRCM
    HWREG(CM_WKUP_UART0_CLKCTRL);
    while ((HWREG(CM_WKUP_UART0_CLKCTRL) & 0x03) != 0x02);
    
    //multiplexação para os pinos rx e tx serial
    HWREG(CONTROL_MODULE_BASE + CONF_UART0_RXD) = 0x20; //tx para receber
    HWREG(CONTROL_MODULE_BASE + CONF_UART0_TXD) = 0x00; //rx para transmitir
    
    //baund rate
    HWREG(SOC_UART_0_REGS + UART_MDR1) = 0x07; //desativando temporariamente

    HWREG(SOC_UART_0_REGS + UART_LCR) |= (1 << 7);

    //Divisor = 48000000 / (16 * 115200) = 26
    HWREG(SOC_UART_0_REGS + UART_DLL) = 0x1A; //parte alta 26
    HWREG(SOC_UART_0_REGS + UART_DLH) = 0x00; //parte baixa

    HWREG(SOC_UART_0_REGS + UART_LCR) = 0x03;//fecha a configuração

    HWREG(SOC_UART_0_REGS + UART_MDR1) = 0x00; //religamos a uart
}

//apenas um caractere por vez 8bites
void uartPutChar(char c){
    while ((HWREG(SOC_UART_0_REGS + UART_LSR) & (1 << 5)) == 0);
    //! Posivel travamento nessa parte o ideal seria resetar a placa caso ocorra
    
    HWREG(SOC_UART_0_REGS + UART_THR) = c; //recebe o caractere
}

//recebe um ponteira e fica em while chamando uartPutChar
void uartPutString(char *str){
    while (*str != '\0') {
        uartPutChar(*str); 
        str++;             
    }
}


//ler o registrador de status UART_LSR se 0 não existe strng se 1 existe string
uint8_t uartAvailable(void){
    if (HWREG(SOC_UART_0_REGS + UART_LSR) & (1 << 0)) {
        return 1; // Tem caractere aguardando
    } else {
        return 0; // Caixa vazia
    }
}
//se uartAvailable verdadeiro vai no registrador de recepção implementar na main
//retorna o primeiro BYTE, ele desaparece automaticamente
char uartGetChar(void){
    return (char)HWREG(SOC_UART_0_REGS + UART_RHR);
}