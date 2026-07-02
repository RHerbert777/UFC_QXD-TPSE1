/*
* P9.14 
*/
#include <stdint.h>
#include <stdio.h>
#include "PWM.h"

int main(){
    //CONFIG WATCH DOG
    disable_wdt();
    
    PRCM_ENABLE_MODE(CM_PER_EPWMSS1_CLKCTRL);
    
    //Direção
    HWREG(SOC_CONTROL_REGS + CM_CONF_GPMC_A2) = 6;
    
    //Desbloqueando trava clock geral
    HWREG(SOC_CONTROL_REGS + PWMSS_CTRL) |= (1 << 1);

    //Setando buzzer
    SET_BUZZER();

    //Setando frequancia do buzzer
    
    //Controle
    while (1){
        PWM_SET_FREQUENCY(440);

        PWM_PLAY();
        
        delay(1000000);
        
        PWM_STOP();

        delay(1000000);

        PWM_SET_FREQUENCY(220);
        PWM_PLAY();

        delay(1000000);
        PWM_STOP();
    }
    

    return 0;
}