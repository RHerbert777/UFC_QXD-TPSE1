#include "gpio.h"
#include <stdint.h>

//Desabilita o whach dog timmer 
void disable_wdt(void) {
    unsigned int addr_wspr = SOC_WDT_1_REGS + WDT_WSPR;
    unsigned int addr_wwps = SOC_WDT_1_REGS + WDT_WWPS;

    HWREG(addr_wspr) = 0xAAAA;

    while ( (HWREG(addr_wwps) & (1 << 4)) != 0 );

    HWREG(addr_wspr) = 0x5555;

    while ( (HWREG(addr_wwps) & (1 << 4)) != 0 );
}

//Ativa o Clock no PRCM do modulo
void AT_MOD(unsigned int MODULO) {
    HWREG(SOC_CM_PER_REGS + MODULO) |= (0x2 << 0);
    
    while ((HWREG(SOC_CM_PER_REGS + MODULO) & (3 << 16)) != 0) {}
    return;
}
//Configura a Multiplexação do pino
void MUX_CONFIG(uint32_t CONFIG, unsigned int MODULO){
    uint32_t temp = HWREG(SOC_CONTROL_REGS + MODULO);
    temp &= ~0x3F;            
    temp |= (CONFIG & 0x3F); 
    HWREG(SOC_CONTROL_REGS + MODULO) = temp;
    return;
}
//Seta a direção do pino se sera entrada ou saida
void CONF_DIR(unsigned int GPIO_BASE, uint32_t pin, uint32_t dir){
    unsigned int addr_temp = GPIO_BASE + GPIO_OE;
    unsigned int val_temp = HWREG(addr_temp);
    val_temp &= ~(1<<pin);
    val_temp |= (dir<<pin);
    HWREG(addr_temp) = val_temp;
    return;
}
//Seta a capacidade de debounce no pino
void GPIO_DEBOUNCE_PIN_ENABLE(unsigned int GPIO_BASE, uint8_t pin) {
    HWREG(GPIO_BASE + GPIO_DEBOUNCENABLE) |= (1 << pin);
}
//Seta o tempo do DEBOUNCE
void GPIO_DEBOUNCE_TIME(unsigned int GPIO_BASE, uint8_t tempo) {
    HWREG(GPIO_BASE + GPIO_DEBOUNCINGTIME) = tempo;
}

// Habilita o relógio opcional de Debounce (32 kHz) para o módulo inteiro
void GPIO_DEBOUNCE_CLOCK_ENABLE(unsigned int MODULO) {
   
    HWREG(SOC_CM_PER_REGS + MODULO) |= (1 << 18);
    
    while ((HWREG(SOC_CM_PER_REGS + MODULO) & (1 << 18)) == 0) {
        // Aguarda a confirmação de hardware
    }
}

//Seta o pin como 1 (alto)
void GPIO1_SET_PIN(unsigned int GPIO_BASE, uint32_t pin){
    unsigned int addr_temp = GPIO_BASE + GPIO_SETDATAOUT;
    unsigned int val_temp = 1<<pin;
    HWREG(addr_temp) |= val_temp;
    return;
}

//Seta o pin como 0 (baixo)
void GPIO1_CLEAN_PIN(unsigned int GPIO_BASE, uint32_t pin){
    unsigned int addr_temp = GPIO_BASE + GPIO_CLEANDATAOUT;
    unsigned int val_temp = 1<<pin;
    HWREG(addr_temp) = val_temp;//altera o pino especifico
}

//Faz a leitura do pino de GPIO em modo entrada
uint32_t GPIO_READ_PIN(unsigned int GPIO_BASE, uint32_t pin) {
    uint32_t estado = HWREG(GPIO_BASE + GPIO_DATAIN);
    
    if (estado & (1 << pin)) {
        return 1;
    } else {
        return 0;
    }
}