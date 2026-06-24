/*
? DISPLAY
* CLK P9.16 gpmc_a3
* DIO P9.23 gpmc_a1
*/
#include "display.h"
#include "gpio.h"
#include "time.h"
#include "soc_AM335x.h"

int main(void) {
    disable_wdt(); 
    
    AT_MOD(CKM_PER_GPIO1_CLKCTRL);
    
    // Inicializa o display
    TM1637_Init();
    
    // Limpa qualquer lixo da memória do display
    TM1637_Limpar_Tela();

    // Vamos escrever o ano! (2026)
    TM1637_Escrever_Digito(TM1637_POS_1, 2);
    TM1637_Escrever_Digito(TM1637_POS_2, 0);
    TM1637_Escrever_Digito(TM1637_POS_3, 2);
    TM1637_Escrever_Digito(TM1637_POS_4, 6);

    while(1) {
        // Laço infinito livre!
    }
}