/* 
* P8.28 GPIO2_24 LED CM_CONF_LCD_PCLK
*/

#include "soc_AM335x.h"
#include "timer.h"
#include "gpio.h"
#include "uart.h"

int main(){
    disable_wdt();
    
    timerSetup();
    
    //Clock
    AT_MOD(CKM_PER_GPIO2_CLKCTRL);
    MUX_CONFIG(0x7, CM_CONF_LCD_PCLK);
    
    CONF_DIR(SOC_GPIO_2_REGS, 24, GPIO_OUTPUT);
    
        uartPutString("\r\n--------Pratica_04---------\r\n");
    uartPutString("Tempo declarado em HEARTS, 0,5hz corresponde a 1 segundo\r\n");
    uartPutString("Escolha uma frequencia:\r\n");
    uartPutString("[1]: 0,05hz\r\n");
    uartPutString("[2]: 0,1hz\r\n");
    uartPutString("[3]: 0,5hz\r\n");
    uartPutString("[4]: 1hz\r\n");
    uartPutString("[5]: 2,5hz\r\n");
    uartPutString("[6]: 5hz\r\n");
    uartPutString("[7]: 10hz\r\n");
    uartPutString("[8]: 16,7hz\r\n");
    uartPutString("[9]: 50hz\r\n");
    uartPutString("[0]: 500hz\r\n");
    uartPutString("---------------------------\r\n");
    uint32_t tempo_led = 1000; //inicia com 1 segundo


    //f = 1000/(2*tempo_led)
    //0,05hz
    //0,1hz
    //0,5
    //1hz
    //2,5hz
    //5hz
    //10hz
    //16,66667hz
    //50hz
    //500hz
    while (1){
        
        if (uartAvailable()) {
            char comando = uartGetChar();
            
            if      (comando == '1') {
                tempo_led = 10000;
                uartPutString("-> Frequencia alterada para: 0,05hz\r\n\n");
            }
            else if (comando == '2'){ 
                tempo_led = 5000;
                uartPutString("-> Frequencia alterada para: 0,1hz\r\n\n");
            }
            else if (comando == '3'){ 
                tempo_led = 1000;
                uartPutString("-> Frequencia alterada para: 0,5hz\r\n\n");
            }
            else if (comando == '4'){ 
                tempo_led = 500;
                uartPutString("-> Frequencia alterada para: 1hz\r\n\n");
            }
            else if (comando == '5'){ 
                tempo_led = 200;
                uartPutString("-> Frequencia alterada para: 2,5hz\r\n\n");
            }
            else if (comando == '6'){ 
                tempo_led = 100;
                uartPutString("-> Frequencia alterada para: 5hz\r\n\n");
            }
            else if (comando == '7'){ 
                tempo_led = 50;
                uartPutString("-> Frequencia alterada para: 10hz\r\n\n");
            }
            else if (comando == '8'){ 
                tempo_led = 30;
                uartPutString("-> Frequencia alterada para: 16,7hz\r\n\n");
            }
            else if (comando == '9'){ 
                tempo_led = 10;
                uartPutString("-> Frequencia alterada para: 50hz\r\n\n");
            }else if(comando == '0'){
                tempo_led = 1;
                uartPutString("-> Frequencia alterada para: 500hz\r\n\n");
            }
        }

        GPIO_SET_PIN(SOC_GPIO_2_REGS, 24); 
        
        mtimerDelay(tempo_led);

        GPIO_CLEAN_PIN(SOC_GPIO_2_REGS, 24);
        
        mtimerDelay(tempo_led);
    }
        
    return 0;
}


    //timer com ms
    /*
    uartPutString("\r\n--------Pratica_04---------\r\n");
    uartPutString("Tempo declarado em ms, 1000ms corresponde a 1 segundo\r\n");
    uartPutString("Escolha uma frequencia:\r\n");
    uartPutString("[1]: 10000ms\r\n");
    uartPutString("[2]: 5000ms\r\n");
    uartPutString("[3]: 1000ms\r\n");
    uartPutString("[4]: 500ms\r\n");
    uartPutString("[5]: 200ms\r\n");
    uartPutString("[6]: 100ms\r\n");
    uartPutString("[7]: 50ms\r\n");
    uartPutString("[8]: 30ms\r\n");
    uartPutString("[9]: 10ms\r\n");
    uartPutString("---------------------------\r\n");
    uint32_t tempo_led = 1000; //inicia com 1 segundo


    //f = 1000/(2*tempo_led)
    //0,05hz
    //0,1hz
    //0,5
    //1hz
    //2,5hz
    //5hz
    //10hz
    //16,66667hz
    //50hz
    while (1){
        
        if (uartAvailable()) {
            char comando = uartGetChar();
            
            if      (comando == '1') {
                tempo_led = 10000;
                uartPutString("-> Tempo alterado para 10000ms\r\n\n");
            }
            else if (comando == '2'){ 
                tempo_led = 5000;
                uartPutString("-> Tempo alterado para 5000ms\r\n\n");
            }
            else if (comando == '3'){ 
                tempo_led = 1000;
                uartPutString("-> Tempo alterado para 1000ms\r\n\n");
            }
            else if (comando == '4'){ 
                tempo_led = 500;
                uartPutString("-> Tempo alterado para 500ms\r\n\n");
            }
            else if (comando == '5'){ 
                tempo_led = 200;
                uartPutString("-> Tempo alterado para 200ms\r\n\n");
            }
            else if (comando == '6'){ 
                tempo_led = 100;
                uartPutString("-> Tempo alterado para 100ms\r\n\n");
            }
            else if (comando == '7'){ 
                tempo_led = 50;
                uartPutString("-> Tempo alterado para 50ms\r\n\n");
            }
            else if (comando == '8'){ 
                tempo_led = 30;
                uartPutString("-> Tempo alterado para 30ms\r\n\n");
            }
            else if (comando == '9'){ 
                tempo_led = 10;
                uartPutString("-> Tempo alterado para 10ms\r\n\n");
            }
        }

        GPIO_SET_PIN(SOC_GPIO_2_REGS, 24); 
        
        mtimerDelay(tempo_led);

        GPIO_CLEAN_PIN(SOC_GPIO_2_REGS, 24);
        
        mtimerDelay(tempo_led);
    }
        
    return 0;
}
*/