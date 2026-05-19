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

void CONF_DIR(uint32_t pin, uint32_t dir){
    unsigned int addr_temp = SOC_GPIO_1_REGS + GPIO_OE;
    unsigned int val_temp = HWREG(addr_temp);
    val_temp &= ~(1<<pin);
    val_temp |= (dir<<pin);
    HWREG(addr_temp) = val_temp;
}

//Seta o pin como 1 (alto)
void GPIO1_SET_PIN(uint32_t pin){
    unsigned int addr_temp = SOC_GPIO_1_REGS + GPIO_SETDATAOUT;
    unsigned int val_temp = 1<<pin;
    HWREG(addr_temp) |= val_temp; //altera o pino especifico
}

//Seta o pin como 0 (baixo)
void GPIO1_CLEAN_PIN(uint32_t pin){
    unsigned int addr_temp = SOC_GPIO_1_REGS + GPIO_CLEANDATAOUT;
    unsigned int val_temp = 1<<pin;
    HWREG(addr_temp) = val_temp;
}