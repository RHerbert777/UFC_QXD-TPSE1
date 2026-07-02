#include "metronomo.h"

//teste do display 8888
void Dispaly_test(){
    TM1637_Escrever_Digito_Invertido(TM1637_POS_1, 8);

    TM1637_Escrever_Digito_Invertido(TM1637_POS_2, 8);

    TM1637_Escrever_Digito_Invertido(TM1637_POS_3, 8);

    TM1637_Escrever_Digito_Invertido(TM1637_POS_4, 8);
}

//Acender todos os leds
void LEDs_test(){
    GPIO_SET_PIN(SOC_GPIO_1_REGS, 6);
    GPIO_SET_PIN(SOC_GPIO_1_REGS, 2);
    GPIO_SET_PIN(SOC_GPIO_1_REGS, 14);
    GPIO_SET_PIN(SOC_GPIO_1_REGS, 29);
    GPIO_SET_PIN(SOC_GPIO_1_REGS, 12);
}

//MENU SET_BPM =========================================================
void start() {
    // CAMADA 1: CLOCKS E ENERGIA (Ligar os disjuntores da casa)
    AT_MOD(CKM_PER_GPIO1_CLKCTRL);
    AT_MOD(CKM_PER_GPIO2_CLKCTRL);
    PRCM_Debounce_Enable(CKM_PER_GPIO1_CLKCTRL); // Relógio de filtro dos botões
    PRCM_Debounce_Enable(CKM_PER_GPIO2_CLKCTRL);

    //MUX LEDs
    MUX_CONFIG(0x7, CM_conf_gpmc_ad6); 
    CONF_DIR(SOC_GPIO_1_REGS, 6, GPIO_OUTPUT);
    
    MUX_CONFIG(0x7, CM_conf_gpmc_ad2); 
    CONF_DIR(SOC_GPIO_1_REGS, 2, GPIO_OUTPUT);
    
    MUX_CONFIG(0x7, CM_conf_gpmc_ad14); 
    CONF_DIR(SOC_GPIO_1_REGS, 14, GPIO_OUTPUT);
    
    MUX_CONFIG(0x7, CM_conf_gpmc_csn0);
    CONF_DIR(SOC_GPIO_1_REGS, 29, GPIO_OUTPUT);
    
    MUX_CONFIG(0x7, CM_conf_gpmc_ad12);
    CONF_DIR(SOC_GPIO_1_REGS, 12, GPIO_OUTPUT);
    
    MUX_CONFIG(0x7, CM_conf_gpmc_a0);
    CONF_DIR(SOC_GPIO_1_REGS, 16, GPIO_OUTPUT);

    //MUX BOTTONS
    MUX_CONFIG(0x27, CM_conf_gpmc_clk);
    CONF_DIR(SOC_GPIO_2_REGS, 1,  GPIO_INPUT); // SET
    
    MUX_CONFIG(0x27, CM_conf_gpmc_wen);   
    CONF_DIR(SOC_GPIO_2_REGS, 4,  GPIO_INPUT); // UP
    
    // Novos pinos seguros para START e DOWN
    MUX_CONFIG(0x27, CM_conf_gpmc_ad15);  
    CONF_DIR(SOC_GPIO_1_REGS, 15, GPIO_INPUT); // START (P8.15)
    
    MUX_CONFIG(0x27, CM_conf_gpmc_ben1);  
    CONF_DIR(SOC_GPIO_1_REGS, 28, GPIO_INPUT); // DOWN (P9.12)
    
    // Periféricos Complexos
    APAGAR_TODOS_LEDS_COLUNA(); //inicializando apagado

    TM1637_Init();

    PRCM_ENABLE_MODE(CM_PER_EPWMSS1_CLKCTRL);//ativamos o prcm

    HWREG(SOC_CONTROL_REGS + CM_CONF_GPMC_A2) = 6; //Mux Buzzer
    SET_BUZZER(CM_PER_EPWMSS1_CLKCTRL);
    PWM_SET_FREQUENCY(440);

    // Interupçoes
    gpioDebounceSetup(SOC_GPIO_1_REGS, 15, 255);
    gpioInterruptSetup(SOC_GPIO_1_REGS, 15, EDGE_RISING);

    gpioDebounceSetup(SOC_GPIO_2_REGS, 1, 255);
    gpioInterruptSetup(SOC_GPIO_2_REGS, 1, EDGE_RISING);

    gpioDebounceSetup(SOC_GPIO_2_REGS, 4, 255);
    gpioInterruptSetup(SOC_GPIO_2_REGS, 4, EDGE_RISING);

    gpioDebounceSetup(SOC_GPIO_1_REGS, 28, 255);
    gpioInterruptSetup(SOC_GPIO_1_REGS, 28, EDGE_RISING);

    IntAINTCInit(); // Liga o cérebro das interrupções
    
    AINTC_Destrancar_Interrupcao(SOC_GPIO_1_REGS);
    AINTC_Destrancar_Interrupcao(SOC_GPIO_2_REGS);

    // Inicializa contagem de tempo    
    Timer7_Start_System_Tick();

    DESLIGAR_WATCHDOG_DE_FABRICA();
    //enable_wdt(5);
    return;
}


//VISUAL LEDS ====================================================
void LED_SIMPLES_ON(){
    GPIO_SET_PIN(SOC_GPIO_1_REGS, 16);
}

void LED_SIMPLES_OFF(){
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS,16);
}

//Limpamos a coluna de led
void APAGAR_TODOS_LEDS_COLUNA(void) {
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, 6);
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, 2);
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, 14);
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, 29);
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, 12);
}

void ACENDER_LED_EXCLUSIVO(uint8_t led_ativo) {
    APAGAR_TODOS_LEDS_COLUNA(); 
    
    if (led_ativo == 1)      GPIO_SET_PIN(SOC_GPIO_1_REGS, 6); //base da pilha
    else if (led_ativo == 2) GPIO_SET_PIN(SOC_GPIO_1_REGS, 2);
    else if (led_ativo == 3) GPIO_SET_PIN(SOC_GPIO_1_REGS, 14);
    else if (led_ativo == 4) GPIO_SET_PIN(SOC_GPIO_1_REGS, 29);
    else if (led_ativo == 5) GPIO_SET_PIN(SOC_GPIO_1_REGS, 12); //topo da pilha
}

//Processa a animação da coluna de leds 
void Processar_Animacao_Coluna(void) {
    volatile uint32_t tempo_corrido = sistema_ms - tempo_ultimo_compasso;
    //Maquina de estados
    static uint8_t led_atual = 0; 
    uint8_t novo_led = 0;

    //Garantimos o pulso de 50ms no ultimo led 
    if (tempo_corrido < 50) { 
        novo_led = 5;
    } 
    
    else {
        volatile uint32_t tempo_por_passo = intervalo_ms / 4; 
        
        if      (tempo_corrido < (tempo_por_passo * 1)) novo_led = 1;
        else if (tempo_corrido < (tempo_por_passo * 2)) novo_led = 2;
        else if (tempo_corrido < (tempo_por_passo * 3)) novo_led = 3;
        else                                            novo_led = 4;
    }
    
    //Garentimos que so ira trocar se ouver mudanças
    if (novo_led != led_atual) {
        ACENDER_LED_EXCLUSIVO(novo_led); 
        led_atual = novo_led;
    }
}

//Apresenta o valor de BPM no formato | 120|
void DISPLAY_CONVERSAO_BPM(uint16_t valor_bpm){
    TM1637_Limpar_Tela();

    uint8_t centenas = (valor_bpm / 100) % 10;
    uint8_t dezenas  = (valor_bpm / 10) % 10;
    uint8_t unidades = valor_bpm % 10;

    if (centenas == 0) {
        TM1637_Escrever_Digito_Invertido(TM1637_POS_2, 10); //Apagamos a centena
    } else {
        TM1637_Escrever_Digito_Invertido(TM1637_POS_2, centenas); 
    }

    TM1637_Escrever_Digito_Invertido(TM1637_POS_3, dezenas);
    TM1637_Escrever_Digito_Invertido(TM1637_POS_4, unidades);
}

// Apresenta no display no formato |F - - 1|
void DISPLAY_TELA_VISUAL(uint8_t modo) {
    TM1637_Limpar_Tela();

    TM1637_Escrever_Segmentos_Brutos_Invertido(0, 0x71);
    
    TM1637_Escrever_Segmentos_Brutos_Invertido(1, 0x40); 
    
    TM1637_Escrever_Segmentos_Brutos_Invertido(2, 0x40); 

    TM1637_Escrever_Digito_Invertido(3, modo); 
}

// Apresenta o display para o modo compasso: |C - - 4|
void DISPLAY_CONVERSAO_COMPASSO(uint8_t compasso) {
    TM1637_Limpar_Tela();

    TM1637_Escrever_Segmentos_Brutos_Invertido(0, 0x39);
   
    TM1637_Escrever_Segmentos_Brutos_Invertido(1, 0x40);

    TM1637_Escrever_Segmentos_Brutos_Invertido(2, 0x40);

    TM1637_Escrever_Digito_Invertido(3, compasso);
}

//Interupção que seta o bpm e o compaço do metronomo
void SET_BPM() {
    PWM_STOP();
    flag_SET = false;
    flag_UP = false;
    flag_DOWN = false;
    flag_start = false;

    uint8_t tela_edicao = 0;
    
    DISPLAY_CONVERSAO_BPM(bpm); // Display recebe bpm atual
    
    uartPutString("Entrou em SET_BPM\r\n");
    
    while (1) {
        // BOTÃO SET: Troca de Tela
        if (flag_SET == true) {
            flag_SET = false;
            tela_edicao++;
            if (tela_edicao > 2) tela_edicao = 0; //limitando telas
            
            uartPutString("Mudou de tela!\r\n");
            
            if (tela_edicao == 0) DISPLAY_CONVERSAO_BPM(bpm);
            else if (tela_edicao == 1) DISPLAY_CONVERSAO_COMPASSO(compasso);
            else if (tela_edicao == 2) DISPLAY_TELA_VISUAL(modo_visual);
        }
        
        // TELA 0: Configuração de BPM
        if (tela_edicao == 0) {
            if (flag_UP == true) {
                flag_UP = false;
                bpm++;
                if (bpm > 300) bpm = 40;
                DISPLAY_CONVERSAO_BPM(bpm);
            }
            if (flag_DOWN == true) {
                flag_DOWN = false;
                if (bpm <= 40) bpm = 300;
                else bpm--;
                DISPLAY_CONVERSAO_BPM(bpm);
            }
        }
        
        // TELA 1: Configuração de COMPASSO
        if (tela_edicao == 1) {
            if (flag_UP == true) {
                flag_UP = false;
                compasso++;
                if (compasso > 7) compasso = 1; 
                DISPLAY_CONVERSAO_COMPASSO(compasso);
            }
            if (flag_DOWN == true) {
                flag_DOWN = false;
                if (compasso <= 1) compasso = 7; 
                else compasso--;
                DISPLAY_CONVERSAO_COMPASSO(compasso);
            }
        }

        // TELA 2: MODO VISUAL LED
        if (tela_edicao == 2) {
            if (flag_UP == true) {
                flag_UP = false;
                modo_visual++;
                if (modo_visual > 3) modo_visual = 1;
                DISPLAY_TELA_VISUAL(modo_visual); 
            }
            if (flag_DOWN == true) {
                flag_DOWN = false;
                if (modo_visual <= 1) modo_visual = 3; 
                else modo_visual--;
                DISPLAY_TELA_VISUAL(modo_visual); 
            }
        }

        // BOTÃO START: Sair e Salvar
        if (flag_start == true) {
            flag_start = false;
            uartPutString("Saindo do Menu... Iniciando Metronomo\r\n");
            
            batida_atual = 1;
            intervalo_ms = 60000 / bpm;
            tempo_ultimo_compasso = sistema_ms - intervalo_ms; 
            estado_metronomo = TOCANDO; 
            
            TM1637_Limpar_Tela();

            DISPLAY_CONVERSAO_BPM(bpm);

            return; 
        }
    }
}