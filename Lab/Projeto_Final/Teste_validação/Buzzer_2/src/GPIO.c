#include "gpio.h"
#include "soc_AM335x.h"
#include "hw_types.h"
#include <stdint.h>

//Ativa o Clock no PRCM do modulo
void AT_MOD(unsigned int MODULO) {
    HWREG(SOC_CM_PER_REGS + MODULO) |= (0x2 << 0);
    
    while ((HWREG(SOC_CM_PER_REGS + MODULO) & (3 << 16)) != 0) {}
}

//Seta a configuração especifica que garante a filtragem do ruido
void GPIO_DEBOUNCE_ENABLE(unsigned int MODULO) {
    HWREG(SOC_CM_PER_REGS + MODULO) |= (1 << 18);

    while ((HWREG(SOC_CM_PER_REGS + MODULO) & (1 << 18)) == 0) {
        // Aguarda a confirmação de hardware
    }
}

//Configura a Multiplexação do pino
void MUX_CONFIG(uint32_t CONFIG, unsigned int MODULO){
    uint32_t temp = HWREG(SOC_CONTROL_REGS + MODULO);
    temp &= ~0x3F;            
    temp |= (CONFIG & 0x3F); 
    HWREG(SOC_CONTROL_REGS + MODULO) = temp;
}

//Seta a direção do pino se sera entrada ou saida
void CONF_DIR(unsigned int GPIO_BASE, uint32_t pin, uint32_t dir){
    unsigned int addr_temp = GPIO_BASE + GPIO_OE;
    unsigned int val_temp = HWREG(addr_temp);
    val_temp &= ~(1<<pin);
    val_temp |= (dir<<pin);
    HWREG(addr_temp) = val_temp;
}

//Seta o pin como 1 (alto)
void GPIO_SET_PIN(unsigned int GPIO_BASE, uint32_t pin){
    unsigned int addr_temp = GPIO_BASE + GPIO_SETDATAOUT;
    unsigned int val_temp = 1<<pin;
    HWREG(addr_temp) |= val_temp;
}

//Seta o pin como 0 (baixo)
void GPIO_CLEAN_PIN(unsigned int GPIO_BASE, uint32_t pin){
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