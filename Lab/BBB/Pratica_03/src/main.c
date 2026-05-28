/*
* GPIO2 - 0x481A_C000
* P8 PIN 18 GPIO2_1  BUTTON_1
* P8 PIN 27 GPIO2_22 BUTTON_2
* P8 PIN 28 GPIO2_24 LED 
*  
*/
#include <stdint.h>

#include "gpio.h"

int main(){
    //CONFIG WATCH DOG
    disable_wdt();
    
    //definindo o clock modulo gpio1 e gpio2
    AT_MOD(CKM_PER_GPIO1_CLKCTRL);
    AT_MOD(CKM_PER_GPIO2_CLKCTRL);

    GPIO_DEBOUNCE_ENABLE(CKM_PER_GPIO2_CLKCTRL);

    //Ativando a multiplexação do pino ---------------------
    //LED
    MUX_CONFIG(0x07, CM_CONF_GPMC_A5);
    MUX_CONFIG(0x07, CM_CONF_GPMC_A6);
    MUX_CONFIG(0x07, CM_CONF_GPMC_A7);
    MUX_CONFIG(0x07, CM_CONF_GPMC_A8);
    MUX_CONFIG(0X07, CM_CONF_LCD_PCLK);  //PIN 28
    
    //BUTTON
    MUX_CONFIG(0X27, CM_CONF_LCD_VSYNC); //PIN 27
    MUX_CONFIG(0x27, CM_CONF_GPMC_CLK);  //PIN 18

    //Setado pinos para saida
    for (uint8_t i = 21; i < 25; i++){
        CONF_DIR(SOC_GPIO_1_REGS,i,0);
    }
    
    CONF_DIR(SOC_GPIO_2_REGS,24,0);

    //Setado gpio como entrada 1
    CONF_DIR(SOC_GPIO_2_REGS,1,1);
    CONF_DIR(SOC_GPIO_2_REGS,22,1);

    uint8_t ESTADO_GERAL = 0; //Simples 0 complexo 1
    uint8_t ESTADO_GERAL_ANTERIOR = 0;
    uint8_t ESTADO_GERAL_ATUAL = 0;

    uint8_t ESTADO_PISCA = 0;
    uint8_t ESTADO_ATUAL = 0;
    uint8_t ESTADO_ANTERIOR = 0;

    //Controle
    while (1){
        ESTADO_ATUAL = GPIO_READ_PIN(SOC_GPIO_2_REGS,1);
        ESTADO_GERAL_ATUAL = GPIO_READ_PIN(SOC_GPIO_2_REGS,22);
        
        //Necesario para garantir que a atualização do estado pisca
        if (ESTADO_ATUAL == 1 && ESTADO_ANTERIOR == 0){
            ESTADO_PISCA++;

            if(ESTADO_PISCA > 2){
                ESTADO_PISCA = 0;
            }
        }
        
        if (ESTADO_GERAL_ATUAL == 1 && ESTADO_GERAL_ANTERIOR == 0){
            ESTADO_GERAL++;

            if(ESTADO_GERAL > 1){
                ESTADO_GERAL = 0;
            }
        }

        //O ontem se torna o hoje
        ESTADO_ANTERIOR = ESTADO_ATUAL;
        ESTADO_GERAL_ANTERIOR = ESTADO_GERAL_ATUAL;

        switch (ESTADO_GERAL){
        
        case 0: //caso geral simples leds internos
            switch (ESTADO_PISCA){
                case 0:
                    SEQUENCIA_0_0 ();
                    break;
                case 1:
                    SEQUENCIA_0_1();
                    break;

                case 2:
                    SEQUENCIA_0_2 ();
                    break;
                
                }
        break;
        case 1:
                switch (ESTADO_PISCA){
                case 0:
                    SEQUENCIA_1_0 ();
                    break;
                case 1:
                    SEQUENCIA_1_1();
                    break;

                case 2:
                    SEQUENCIA_1_2 ();
                    break;
                
                }
        break;
        }
        delay(10);
    }
    return 0;
}