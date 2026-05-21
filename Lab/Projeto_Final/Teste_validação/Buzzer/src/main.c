/* Objetivo utilizar o pwm para gerar uma onda na frequencia alta e baixa 220 440 hertz
* 
*/
#include <stdint.h>
#include <stdio.h>
#include "BBB_REGS.h"
#include "Func.h"

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
    PWM_SET_FREQUENCY(440);

    //Controle
    while (1){
        PWM_PLAY();
        
        delay(10000000);
        
        PWM_STOP();

        delay(10000000);
    }
    

    return 0;
}