/*
 * =============================================================================
 * PRÁTICA 05 - MULTIPLEXAÇÃO (MODO 7)
 * =============================================================================
 *
 * --- BOTÕES (ENTRADAS COM INTERRUPÇÃO) ---
 * Button 1 (Menu)     -> PINO: P9_15 | PORTA: gpio1_16 (GPIO 48) | MUX: conf_gpmc_a0  
 * Button 2 (Alternar) -> PINO: P9_12 | PORTA: gpio1_28 (GPIO 60) | MUX: conf_gpmc_ben1
 *
 * --- LEDs (SAÍDAS) ---
 * LED 1               -> PINO: P8_08 | PORTA: gpio2_3  (GPIO 67) | MUX: conf_gpmc_oen_ren 
 * LED 2               -> PINO: P8_10 | PORTA: gpio2_4  (GPIO 68) | MUX: conf_gpmc_wen     
 * LED 3               -> PINO: P8_18 | PORTA: gpio2_1  (GPIO 65) | MUX: conf_gpmc_clk     
 * ============================================================================= */


#include "interupt.h"
#include "timer.h"
#include "gpio.h"
#include "pad.h"
#include "uart.h"

void Sequencialed(unsigned int TM){
    GPIO_SET_PIN  (SOC_GPIO_2_REGS,3);
    delay_ms_interrupt(TM);
    GPIO_CLEAN_PIN(SOC_GPIO_2_REGS,3);

    GPIO_SET_PIN  (SOC_GPIO_2_REGS,4);
    delay_ms_interrupt(TM);
    GPIO_CLEAN_PIN(SOC_GPIO_2_REGS,4);

    GPIO_SET_PIN  (SOC_GPIO_2_REGS,1);
    delay_ms_interrupt(TM);
    GPIO_CLEAN_PIN(SOC_GPIO_2_REGS,1);
    return;
}

void Sequencialed2(unsigned int TM){
    GPIO_SET_PIN  (SOC_GPIO_2_REGS, 1);
    delay_ms_interrupt(TM);
    GPIO_CLEAN_PIN(SOC_GPIO_2_REGS, 1);

    GPIO_SET_PIN  (SOC_GPIO_2_REGS, 4);
    delay_ms_interrupt(TM);
    GPIO_CLEAN_PIN(SOC_GPIO_2_REGS, 4);

    GPIO_SET_PIN  (SOC_GPIO_2_REGS, 3);
    delay_ms_interrupt(TM);
    GPIO_CLEAN_PIN(SOC_GPIO_2_REGS, 3);
    return;
}

void desenhar_menu(uint8_t opcao) {
    uartPutString("\r\n=========================================\r\n");
    uartPutString("======         Pratica 5          =======\r\n");
    uartPutString("=========================================\r\n");
    uartPutString("Selecione uma frequencia de pisca:\r\n");
    
    if(opcao == 0) uartPutString("[1]: 1000ms  <--\r\n");
    else           uartPutString("[1]: 1000ms\r\n");
    
    if(opcao == 1) uartPutString("[2]: 500ms   <--\r\n");
    else           uartPutString("[2]: 500ms\r\n");
    
    if(opcao == 2) uartPutString("[3]: 100ms    <--\r\n");
    else           uartPutString("[3]: 100ms\r\n");
}

volatile uint8_t flag_botao_1 = false;
volatile uint8_t flag_botao_2 = false;
volatile uint8_t flag_timer   = false;
int main(){
    disable_wdt();
    uartSetup();

    AT_MOD(CKM_PER_GPIO1_CLKCTRL);
    AT_MOD(CKM_PER_GPIO2_CLKCTRL);
    
    //interupt BUTTON
    PRCM_Debounce_Enable(CKM_PER_GPIO1_CLKCTRL);
    
    MUX_CONFIG(0x27, CM_conf_gpmc_a0);
    CONF_DIR  (SOC_GPIO_1_REGS, 16, GPIO_INPUT);
    gpioDebounceSetup(SOC_GPIO_1_REGS, 16, 255);

    MUX_CONFIG(0x27, CM_conf_gpmc_ben1);
    CONF_DIR  (SOC_GPIO_1_REGS, 28, GPIO_INPUT);
    gpioDebounceSetup(SOC_GPIO_1_REGS, 28, 255);

    timer_PRCM_Setup();//PRCM timer_7
    
    IntAINTCInit(); //ativa o modulo geral de interupção

    timerinteruptsetup(SOC_DMTIMER_7_REGS);//seta interupão do timer 7
    
    //Led 1
    MUX_CONFIG(MODE_7, CM_conf_gpmc_oen_ren);
    CONF_DIR  (SOC_GPIO_2_REGS, 3, GPIO_OUTPUT);

    //Led 2
    MUX_CONFIG(MODE_7, CM_conf_gpmc_wen);
    CONF_DIR  (SOC_GPIO_2_REGS, 4, GPIO_OUTPUT);

    //led 3
    MUX_CONFIG(MODE_7, CM_conf_gpmc_clk);
    CONF_DIR  (SOC_GPIO_2_REGS, 1, GPIO_OUTPUT);

    //tipo de interupção rising ou falling
    gpioInterruptSetup(SOC_GPIO_1_REGS, 16, EDGE_FALLING);
    gpioInterruptSetup(SOC_GPIO_1_REGS, 28, EDGE_RISING);
    
    //variaveis de menu
    volatile uint8_t menu_ativo = false;
    volatile uint8_t opcao_atual = 0;
    volatile uint8_t led_seq = 0;
    volatile unsigned int tempo_led = 1000; //inicializando com o maior tempo
    while (1){
        // ESTADO 1: MENU ABERTO
        if (menu_ativo == true) {
            //seta do menu
            if (flag_botao_2 == true) {
                flag_botao_2 = false; // Limpa a flag
                
                opcao_atual++;
                if (opcao_atual > 2) opcao_atual = 0; // Volta pro topo se passar do 3
                
                desenhar_menu(opcao_atual);
            }
            
            // Se apertar Botão 1 
            if (flag_botao_1 == true) {
                flag_botao_1 = false; // Limpa a flag
                
                // Aplica a escolha na variável de tempo
                if (opcao_atual == 0) tempo_led = 1000;
                else if (opcao_atual == 1) tempo_led = 500;
                else if (opcao_atual == 2) tempo_led = 100;
                
                uartPutString("\r\n>>> Frequencia Atualizada! Retornando...\r\n");
                menu_ativo = false; // Sai do menu e volta a piscar os LEDs!
            }
        }
        // ESTADO 0: PISCANDO LEDs (Normal)
        else {
            
            //sequancia
            if (led_seq == 0){
                Sequencialed(tempo_led);
            }else{
                Sequencialed2(tempo_led);
            }
            
            //abre o menu
            if (flag_botao_1 == true) {
                flag_botao_1 = false; // Limpa a flag
                menu_ativo = true;
                desenhar_menu(opcao_atual);
            }
            
            //Se não tiver menu atualiza modo led
            if (flag_botao_2 == true) {
                flag_botao_2 = false;
                
                led_seq = !led_seq;
                uartPutString("\r\n>>> Sequencia Atualizada! Retornando...\r\n");
            }
        }
    }
    
    return 0;
}