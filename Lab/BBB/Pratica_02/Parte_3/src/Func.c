#include "BBB_REGS.h"
#include "Func.h"
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
//ativa o modulo aneble na tabela PRCM primeiro bit
void AT_MOD(unsigned int MODULO) {
    HWREG(SOC_CM_PER_REGS + MODULO) |= (0x2 << 0);
    
    while ((HWREG(SOC_CM_PER_REGS + MODULO) & (3 << 16)) != 0) {}
}

void GPIO_DEBOUNCE_ENABLE(unsigned int MODULO) {
    HWREG(SOC_CM_PER_REGS + MODULO) |= (1 << 18);
}

void MUX_CONFIG(uint32_t CONFIG, unsigned int MODULO){
    uint32_t temp = HWREG(SOC_CONTROL_REGS + MODULO);
    temp &= ~0x3F;            
    temp |= (CONFIG & 0x3F); 
    HWREG(SOC_CONTROL_REGS + MODULO) = temp;
}

void CONF_DIR(unsigned int GPIO_BASE, uint32_t pin, uint32_t dir){
    unsigned int addr_temp = GPIO_BASE + GPIO_OE;
    unsigned int val_temp = HWREG(addr_temp);
    val_temp &= ~(1<<pin);
    val_temp |= (dir<<pin);
    HWREG(addr_temp) = val_temp;
}

//Seta o pin como 1 (alto)
void GPIO1_SET_PIN(unsigned int GPIO_BASE, uint32_t pin){
    HWREG(GPIO_BASE + GPIO_SETDATAOUT) = (1 << pin);
}

//Seta o pin como 0 (baixo)
void GPIO1_CLEAN_PIN(unsigned int GPIO_BASE, uint32_t pin){
    HWREG(GPIO_BASE + GPIO_CLEANDATAOUT) = (1 << pin);
}