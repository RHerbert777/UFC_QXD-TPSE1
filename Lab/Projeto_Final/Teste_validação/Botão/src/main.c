/*
?  GPIO BUTTON 
*  P8.15 GPIO1_15 - gpmc-ad15 - BUTTON SET + INTERUPÇÃO 
*  P8.18 GPIO2_1 - gpmc-clk-mux0 - BUTTON UP
*  P8.20 GPIO1_31 - gpmc-csn2 - BUTTON DOWN
*  P8.22 GPIO1_5  - gpmc-ad5 - BUTTON START
*/

/*
!Setamos diretamente o debouce apartir dos registradores não precisamos de delay();
!Essa é a parte 1 do nosso codigo principal até o /%/
*/

#include <stdint.h>
#include "gpio.h"

#define GPIO_INPUT 0x1
#define MUX_GPIO 0x27
#define PAD_GPIO_OUTPUT 0

int main(){

    disable_wdt();
    
    //ativando so o necesario
    AT_MOD(CKM_PER_GPIO1_CLKCTRL);
    //!falta definir o nome do pino
    MUX_CONFIG(MUX_GPIO, nome_pino);

    CONF_DIR(SOC_GPIO_1_REGS, 5, GPIO_INPUT);

    GPIO_DEBOUNCE_PIN_ENABLE(SOC_GPIO_1_REGS, 5); //ativando o debouce do pino
    GPIO_DEBOUNCE_TIME(SOC_GPIO_1_REGS, 255); //setando tempo de debounce
    
    uint8_t ESTADO_START = 0;
    uint8_t ESTADO_START_ATUAL = 0;
    uint8_t ESTADO_START_ANTERIOR = 0;
    while(1){
        //Estado de espera do botão start
        while (1){
            ESTADO_START_ATUAL = GPIO_READ_PIN(SOC_GPIO_1_REGS, 5);

            if (ESTADO_START_ATUAL == 1 && ESTADO_START_ANTERIOR == 0){
                ESTADO_START++;
            }

            if (ESTADO_START >= 1){
                break;
            }

            //O ontem vira o hoje
            ESTADO_START_ANTERIOR = ESTADO_START_ATUAL;
        }
        //! /%/
        //Ativo os outros modulos
        //! probleams com configuração do debouce
        AT_MOD(CKM_PER_GPIO2_CLKCTRL);
        MUX_CONFIG(MUX_GPIO, nome_pino);//set
        MUX_CONFIG(MUX_GPIO, nome_pino);//up
        MUX_CONFIG(MUX_GPIO, nome_pino);//down

        CONF_DIR(SOC_GPIO_1_REGS, 31, GPIO_INPUT);
        CONF_DIR(SOC_GPIO_2_REGS, 1, GPIO_INPUT);
        CONF_DIR(SOC_GPIO_1_REGS, 15, GPIO_INPUT);
        
        GPIO_DEBOUNCE_TIME(SOC_GPIO_2_REGS, 255); 
        GPIO_DEBOUNCE_PIN_ENABLE(SOC_GPIO_1_REGS, 31); 
        GPIO_DEBOUNCE_PIN_ENABLE(SOC_GPIO_2_REGS, 1);
        GPIO_DEBOUNCE_PIN_ENABLE(SOC_GPIO_1_REGS, 15);

    }

    return 0;
}