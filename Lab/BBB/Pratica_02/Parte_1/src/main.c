/*
* Pino V15 - GPIO1_21 - 0x4804_C000
*/
#include <stdint.h>
#include "BBB_REGS.h"
#include "Func.h"

int main(){
    //CONFIG WATCH DOG
    disable_wdt();
    
    //definindo o clock
    unsigned int modulo = CKM_PER_GPIO1_CLKCTRL;
    unsigned int setings = (1<<18) | (0x2<<0);
    HWREG(SOC_CM_PER_REGS + modulo) |= setings;
    
    //Ativando o modulo
    unsigned int mode = 7;
    modulo = CM_CONF_GPMC_A5;
    HWREG(SOC_CONTROL_REGS + modulo) |= mode;

    //COFIG DIREÇÃO
    uint32_t pin = 21;
    uint32_t dir = 0;
    unsigned int addr_temp = SOC_GPIO_1_REGS + GPIO_OE;
    unsigned int val_temp = HWREG(addr_temp);
    val_temp &= ~(1<<pin);
    val_temp |= (dir<<pin);
    HWREG(addr_temp) = val_temp;

    //controle
    while (1){
        //Addr = 
        addr_temp = SOC_GPIO_1_REGS + GPIO_SETDATAOUT;
        val_temp = 1<<pin;
        HWREG(addr_temp) |= val_temp; //altera o pino especifico

        delay(1000000);

        addr_temp = SOC_GPIO_1_REGS + GPIO_CLEANDATAOUT;
        val_temp = 1<<pin;
        HWREG(addr_temp) |= val_temp;
        
        delay(1000000);
    }
    

    return 0;
}