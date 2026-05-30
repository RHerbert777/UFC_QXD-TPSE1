/*
* LED0 P8.3 GPIO1_6
* LED1 P8.4 GPIO1_7
* LED2 P8.5 GPIO1_2
* LED3 P8.6 GPIO1_3
* LED4 P8.7 GPIO2_2
*/

#include <stdint.h>
#include "gpio.h"
#include "control_module.h"

#define MUX_GPIO_OUTPUT 0x07
#define PAD_GPIO_OUTPUT 0

int main(){

    disable_wdt();

    AT_MOD(SOC_GPIO_1_REGS);
    AT_MOD(SOC_GPIO_2_REGS);

    MUX_CONFIG(MUX_GPIO_OUTPUT, SOC_GPIO_1_REGS);
    MUX_CONFIG(MUX_GPIO_OUTPUT, SOC_GPIO_2_REGS);

    CONF_DIR(SOC_GPIO_1_REGS, 6, PAD_GPIO_OUTPUT);
    CONF_DIR(SOC_GPIO_1_REGS, 7, PAD_GPIO_OUTPUT);
    CONF_DIR(SOC_GPIO_1_REGS, 2, PAD_GPIO_OUTPUT);
    CONF_DIR(SOC_GPIO_1_REGS, 3, PAD_GPIO_OUTPUT);
    CONF_DIR(SOC_GPIO_2_REGS, 2, PAD_GPIO_OUTPUT);

    while (1){
        
        Sequencia();
        
    }
    


    return;
}