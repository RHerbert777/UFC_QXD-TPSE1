//! Inviavel para nosso projeto ja que a cpu fica ocupada realizando sinal alto e baixo

/* 
* P8.28 GPIO2_24 LED CM_CONF_LCD_PCLK
*/

#include "soc_AM335x.h"
#include "timer.h"
#include "gpio.h"

void alto(uint32_t tempo_led){
    GPIO_SET_PIN(SOC_GPIO_2_REGS, 24); 
    
    microtimerDelay(tempo_led);

    GPIO_CLEAN_PIN(SOC_GPIO_2_REGS, 24);
    
    microtimerDelay(tempo_led);  
}

void baixo(uint32_t tempo_led){
    GPIO_SET_PIN(SOC_GPIO_2_REGS, 24); 
    
    microtimerDelay(tempo_led);

    GPIO_CLEAN_PIN(SOC_GPIO_2_REGS, 24);
    
    microtimerDelay(tempo_led);  
}

int main(){
    disable_wdt();
    
    timerSetup();
    
    //Clock
    AT_MOD(CKM_PER_GPIO2_CLKCTRL);
    MUX_CONFIG(0x7, CM_CONF_LCD_PCLK);
    
    CONF_DIR(SOC_GPIO_2_REGS, 24, 0);//!verificar se é 0 para saida
    
    uint32_t tempo_led = 1136; //isso em milisegundos

    while (1){

        alto(tempo_led);

    }
        
    return 0;
}
