/* 
!pegar o codigo da pratica passada e só alterar para essa pratica 
*   
*
*
*/



#include "timer.h"
#include "gpio.h"
#include "uart.h"

int main(){

    disable_wdt();
   
    AT_MOD(CKM_PER_GPIO1_CLKCTRL);
    AT_MOD(CKM_PER_TIMER7_CLKCTRL); //*ativando o clock timer
    AT_MOD(CM_WKUP_UART0_CLKCTRL); //!conferir gemini deu uma resposta esquisita
    
    MUX_CONFIG(0x7, );
    
    while (1){
        
    }
    
    
    
    return 0;
}