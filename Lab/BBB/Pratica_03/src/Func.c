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
    return;
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

uint32_t GPIO_READ_PIN(unsigned int GPIO_BASE, uint32_t pin) {
    uint32_t estado = HWREG(GPIO_BASE + GPIO_DATAIN);
    
    if (estado & (1 << pin)) {
        return 1;
    } else {
        return 0;
    }
}

void SEQUENCIA_0_0 (){
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 21);
    
    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS,21);
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 22);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 22);
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 23);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 23);
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 24);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 24);
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 23);

    delay(10000000);
    
    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 23);
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 22);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 22);
    GPIO1_SET_PIN  (SOC_GPIO_1_REGS, 21);
}

void SEQUENCIA_0_1(){
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 22);
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 23);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 22);
    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 23);
    
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 21);
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 24);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 21);
    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 24);
}

void SEQUENCIA_0_2(){
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 21);

    delay(10000000);

    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 22);

    delay(10000000);

    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 23);

    delay(10000000);

    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 24);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 21);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 22);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 23);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 24);
}

void SEQUENCIA_1_0 (){
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 21);
    
    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS,21);
    GPIO1_SET_PIN  (SOC_GPIO_1_REGS, 22);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 22);
    GPIO1_SET_PIN  (SOC_GPIO_1_REGS, 23);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 23);
    GPIO1_SET_PIN  (SOC_GPIO_1_REGS, 24);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 24);
    GPIO1_SET_PIN  (SOC_GPIO_2_REGS, 24);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_2_REGS, 24);
    GPIO1_SET_PIN  (SOC_GPIO_1_REGS, 24);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 24);
    GPIO1_SET_PIN  (SOC_GPIO_1_REGS, 23);

    delay(10000000);
    
    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 23);
    GPIO1_SET_PIN  (SOC_GPIO_1_REGS, 22);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 22);
    GPIO1_SET_PIN  (SOC_GPIO_1_REGS, 21);

}

void SEQUENCIA_1_1 (){
    GPIO1_SET_PIN(SOC_GPIO_2_REGS, 24);
    
    delay(10000000);

    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 22);
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 23);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 22);
    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 23);
    
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 21);
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 24);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 21);
    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 24);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_2_REGS, 24);
}

void SEQUENCIA_1_2 (){
    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 21);

    delay(10000000);

    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 22);

    delay(10000000);

    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 23);

    delay(10000000);

    GPIO1_SET_PIN(SOC_GPIO_1_REGS, 24);

    delay(10000000);

    GPIO1_SET_PIN(SOC_GPIO_2_REGS, 24);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 21);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 22);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 23);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_1_REGS, 24);

    delay(10000000);

    GPIO1_CLEAN_PIN(SOC_GPIO_2_REGS, 24);
}