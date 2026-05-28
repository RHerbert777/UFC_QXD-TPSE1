/*
* Pino V15 - GPIO1_21 - 0x4804_C000
* Pino U15 - GPIO1_22 -
* Pino T15 - GPIO1_23 - 
* Pino V16 - GPIO1_24 - 
*/
#include <stdint.h>
#include "BBB_REGS.h"
#include "Func.h"

int main(){
    //CONFIG WATCH DOG
    disable_wdt();
    
    //definindo o clock modulo gpio1
    unsigned int modulo = CKM_PER_GPIO1_CLKCTRL;
    uint32_t setings = (1<<18) | (0x2<<0);
    HWREG(SOC_CM_PER_REGS + modulo) |= setings;
    
    //Ativando o modulo 
    unsigned int mode = 7;
    modulo = CM_CONF_GPMC_A5;
    HWREG(SOC_CONTROL_REGS + modulo) |= mode;

    //COFIG DIREÇÃO -----------------------------------------------------
    //pin 21
    CONF_DIR(21, 0);
    
    //pin 22
    CONF_DIR(22, 0);

    //pin 23
    CONF_DIR(23, 0);
    
    //pin 24
    CONF_DIR(24, 0);
    
    //pin 13
    CONF_DIR(28,0);

    //Controle
    while (1){
    
        GPIO1_SET_PIN(21);

        delay(1000000);

        GPIO1_SET_PIN(22);
        
        delay(1000000);

        GPIO1_SET_PIN(23);
        
        delay(1000000);
        
        GPIO1_SET_PIN(24);
        
        delay(1000000);

        GPIO1_SET_PIN(28);
        
        delay(1000000);

        GPIO1_CLEAN_PIN(24);
        
        delay(1000000);
        
        GPIO1_CLEAN_PIN(23);
        
        delay(1000000);
        
        GPIO1_CLEAN_PIN(22);
        
        delay(1000000);
        
        GPIO1_CLEAN_PIN(21);
        
        delay(1000000);
        
        GPIO1_CLEAN_PIN(28);
        
        delay(1000000);
    }
    

    return 0;
}