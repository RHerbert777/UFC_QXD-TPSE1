#include "metronomo.h"
/*
? GPIO_LEDS: 
? BLOCO coluna
* LED0 P8.3  GPIO1_6 / gpmc_ad6
* LED1 P8.5  GPIO1_7 / gpmc_ad2
* LED2 P8.7  GPIO1_2 / gpmc_advn_ale
* LED3 P8.9  GPIO1_3 / gpmc_b0n_cle
* LED4 P8.11 GPIO2_2 / gpmc_ad13
? BLOCO simples
* LED5 P9.15 GPIO1_16 / gpmc_a0

? BUTTONS:
! BUTTON SET   P8.18 gpio2_1  gpmc_clk_mux0
! BUTTON UP    P8.10 gpio2_4  gpmc_wen
! BUTTON DOWN  P9.12 gpio1_31 gpmc_csn2
! BUTTON START P8.15 gpio1_1  gpmc_ad1

? BUZZER:
* P9.14 

? DISPLAY
* CLK P9.16 gpio1_19  gpmc_a3
* DIO P9.23 gpio1_17 gpmc_a1
*/

void Dispaly_test(){
    TM1637_Escrever_Digito(TM1637_POS_1, 8);

    TM1637_Escrever_Digito(TM1637_POS_2, 8);

    TM1637_Escrever_Digito(TM1637_POS_3, 8);

    TM1637_Escrever_Digito(TM1637_POS_4, 8);
}

void start() {
    // =======================================================
    // CAMADA 1: CLOCKS E ENERGIA (Ligar os disjuntores da casa)
    // =======================================================
    AT_MOD(CKM_PER_GPIO1_CLKCTRL);
    AT_MOD(CKM_PER_GPIO2_CLKCTRL);
    PRCM_Debounce_Enable(CKM_PER_GPIO1_CLKCTRL); // Relógio de filtro dos botões
    PRCM_Debounce_Enable(CKM_PER_GPIO2_CLKCTRL);

    // =======================================================
    // CAMADA 2: MUX, DIREÇÃO E ESTADO INICIAL (Passar os fios)
    // =======================================================
    // LEDs
    MUX_CONFIG(0x7, CM_conf_gpmc_ad6); CONF_DIR(SOC_GPIO_1_REGS, 6, GPIO_OUTPUT);
    MUX_CONFIG(0x7, CM_conf_gpmc_ad2); CONF_DIR(SOC_GPIO_1_REGS, 7, GPIO_OUTPUT);
    MUX_CONFIG(0x7, CM_conf_gpmc_advn_ale); CONF_DIR(SOC_GPIO_1_REGS, 2, GPIO_OUTPUT);
    MUX_CONFIG(0x7, CM_conf_gpmc_ben0_cle); CONF_DIR(SOC_GPIO_1_REGS, 3, GPIO_OUTPUT);
    MUX_CONFIG(0x7, CM_conf_gpmc_ad13); CONF_DIR(SOC_GPIO_2_REGS, 2, GPIO_OUTPUT);
    MUX_CONFIG(0x7, CM_conf_gpmc_a0); CONF_DIR(SOC_GPIO_1_REGS, 16, GPIO_OUTPUT);

    // Botões (Apenas mapear e definir como INPUT)
    // Botões (Novos pinos seguros: P8.15=gpio1_15 e P9.12=gpio1_28)
    // O SET e UP continuam onde estão, pois não conflitam com eMMC
    MUX_CONFIG(0x27, CM_conf_gpmc_clk);   CONF_DIR(SOC_GPIO_2_REGS, 1,  GPIO_INPUT); // SET
    MUX_CONFIG(0x27, CM_conf_gpmc_wen);   CONF_DIR(SOC_GPIO_2_REGS, 4,  GPIO_INPUT); // UP
    // Novos pinos seguros para START e DOWN
    MUX_CONFIG(0x27, CM_conf_gpmc_ad15);  CONF_DIR(SOC_GPIO_1_REGS, 15, GPIO_INPUT); // START (P8.15)
    MUX_CONFIG(0x27, CM_conf_gpmc_ben1);  CONF_DIR(SOC_GPIO_1_REGS, 28, GPIO_INPUT); // DOWN (P9.12)
    // Periféricos Complexos
    //led_init(); // Garante que começam apagados/estado seguro
    TM1637_Init(); // Configura o display com o sistema quieto
    SET_BUZZER(CM_PER_EPWMSS1_CLKCTRL);
    PWM_SET_FREQUENCY(440); // Silencioso até segunda ordem

    // =======================================================
    // CAMADA 3: REGRAS DE INTERRUPÇÃO FÍSICA (Preparar o gatilho)
    // =======================================================
    // Primeiro limpamos ruídos e setamos como os pinos vão reagir
    gpioDebounceSetup(SOC_GPIO_1_REGS, 15, 255);
    gpioInterruptSetup(SOC_GPIO_1_REGS, 15, EDGE_RISING);

    gpioDebounceSetup(SOC_GPIO_2_REGS, 1, 255);
    gpioInterruptSetup(SOC_GPIO_2_REGS, 1, EDGE_RISING);

    gpioDebounceSetup(SOC_GPIO_2_REGS, 4, 255);
    gpioInterruptSetup(SOC_GPIO_2_REGS, 4, EDGE_RISING);

    gpioDebounceSetup(SOC_GPIO_1_REGS, 28, 255);
    gpioInterruptSetup(SOC_GPIO_1_REGS, 28, EDGE_RISING);

    // =======================================================
    // CAMADA 4: CONTROLADOR CENTRAL E DESTRANCAMENTO
    // =======================================================
    IntAINTCInit(); // Liga o cérebro das interrupções
    
    // SÓ AGORA liberamos o caminho dos botões até a CPU
    AINTC_Destrancar_Interrupcao(SOC_GPIO_1_REGS);
    AINTC_Destrancar_Interrupcao(SOC_GPIO_2_REGS);

    //timer_Setup();
    // =======================================================
    // CAMADA 5: O MAESTRO TOMA O CONTROLE DO TEMPO
    // =======================================================
    // O relógio começa a bater APENAS quando todo o hardware está pronto
    Timer7_Start_System_Tick();

    // Se for ligar o Watchdog da sua aplicação, a hora é exatamente AQUI,
    // na última linha, logo antes do return, com um `enable_wdt(5);`

    //enable_wdt(5);
    DESLIGAR_WATCHDOG_DE_FABRICA();
    return;
}

//Converte um numero de 16bites em 3 valores de 8bits e atualiza o display
void DISPLAY_CONVERSAO_BPM(uint16_t valor_bpm){
    uint8_t centenas = (valor_bpm / 100) % 10;
    uint8_t dezenas  = (valor_bpm / 10) % 10;
    uint8_t unidades = valor_bpm % 10;
    
    if (centenas == 0) { 
        TM1637_Escrever_Digito(TM1637_POS_2, 10); //limpamos a centena

    } else {
        TM1637_Escrever_Digito(TM1637_POS_2, centenas);
    }

    TM1637_Escrever_Digito(TM1637_POS_3, dezenas);
    TM1637_Escrever_Digito(TM1637_POS_4, unidades);
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
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, 7);
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, 2);
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, 3);
    GPIO_CLEAN_PIN(SOC_GPIO_2_REGS, 2);
}

void ACENDER_LED_EXCLUSIVO(uint8_t led_ativo) {
    APAGAR_TODOS_LEDS_COLUNA(); 
    
    if (led_ativo == 1)      GPIO_SET_PIN(SOC_GPIO_1_REGS, 6); //base da pilha
    else if (led_ativo == 2) GPIO_SET_PIN(SOC_GPIO_1_REGS, 7);
    else if (led_ativo == 3) GPIO_SET_PIN(SOC_GPIO_1_REGS, 2);
    else if (led_ativo == 4) GPIO_SET_PIN(SOC_GPIO_1_REGS, 3);
    else if (led_ativo == 5) GPIO_SET_PIN(SOC_GPIO_2_REGS, 2); //topo da pilha
}
//Processa a animação da coluna de leds 
void Processar_Animacao_Coluna(void) {
    // Recebemos o tempo para saber o led atual
    uint32_t tempo_corrido = sistema_ms - tempo_ultimo_compasso;
    
    // Fatia o tempo total da batida em 4 janelas idênticas
    uint32_t tempo_por_passo = intervalo_ms / 4; 

    //Define a posição do "pêndulo" baseado no tempo percorrido
    if (tempo_corrido < (tempo_por_passo * 1)) {
        ACENDER_LED_EXCLUSIVO(1); // Início da batida
    } 
    else if (tempo_corrido < (tempo_por_passo * 2)) {
        ACENDER_LED_EXCLUSIVO(2); // Andando 25%
    }
    else if (tempo_corrido < (tempo_por_passo * 3)) {
        ACENDER_LED_EXCLUSIVO(3); // Andando 50% (Meio)
    }
    else if (tempo_corrido < (tempo_por_passo * 4)) {
        ACENDER_LED_EXCLUSIVO(4); // Andando 75%
    }
    else {
        ACENDER_LED_EXCLUSIVO(5); // Final da batida (Quase estourando o próximo pulso)
    }
}

//Apresenta no display no formato L - - 1
void DISPLAY_TELA_VISUAL(uint8_t modo) {
    TM1637_Escrever_Segmentos_Brutos(0, 0x38); // Desenha 'L'
    TM1637_Escrever_Segmentos_Brutos(1, 0x40); // Desenha '-'
    TM1637_Escrever_Segmentos_Brutos(2, 0x40); // Desenha '-'
    
    // Escreve o número do modo na ponta direita
    TM1637_Escrever_Digito(3, modo); 
}
//apresenta o display para o modo compaço somente o numero da esquerda
void DISPLAY_CONVERSAO_COMPASSO(uint8_t compasso) {
    // Posição 0 (Extrema Esquerda): Letra 'C' (Hexadecimal 0x39)
    TM1637_Escrever_Segmentos_Brutos(0, 0x39); 
    
    // Posição 1: Traço '-' (Hexadecimal 0x40)
    TM1637_Escrever_Segmentos_Brutos(1, 0x40); 
    
    // Posição 2: Espaço apagado (Hexadecimal 0x00) para separar visualmente
    TM1637_Escrever_Segmentos_Brutos(2, 0x00); 
    
    // Posição 3 (Extrema Direita): O número do compasso (1 a 7)
    TM1637_Escrever_Digito(3, compasso); 
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
    
    uartPutString("Entrou no menu SET_BPM\r\n");
    
    while (1) {
        // Mantém o Watchdog calmo enquanto editamos os valores
        //ALIMENTAR_CAO_CONTROLADO();
        
        // ==========================================
        // BOTÃO SET: Troca de Tela
        // ==========================================
        if (flag_SET == true) {
            flag_SET = false;
            tela_edicao++;
            if (tela_edicao > 2) tela_edicao = 0; // Corrigido para limitar em 2 (Telas 0, 1 e 2)
            
            uartPutString("Mudou de tela!\r\n");
            
            // Atualiza a tela imediatamente ao alternar
            if (tela_edicao == 0) DISPLAY_CONVERSAO_BPM(bpm);
            else if (tela_edicao == 1) DISPLAY_CONVERSAO_COMPASSO(compasso);
            else if (tela_edicao == 2) DISPLAY_TELA_VISUAL(modo_visual);
        }
        
        // ==========================================
        // TELA 0: Configuração de BPM
        // ==========================================
        if (tela_edicao == 0) {
            if (flag_UP == true) {
                flag_UP = false;
                bpm++;
                if (bpm > 300) bpm = 40;
                uartPutString("BPM Incrementado\r\n");
                DISPLAY_CONVERSAO_BPM(bpm);
            }
            if (flag_DOWN == true) {
                flag_DOWN = false;
                if (bpm <= 40) bpm = 300;
                else bpm--;
                uartPutString("BPM Decrementado\r\n");
                DISPLAY_CONVERSAO_BPM(bpm);
            }
        }
        
        // ==========================================
        // TELA 1: Configuração de COMPASSO
        // ==========================================
        if (tela_edicao == 1) {
            if (flag_UP == true) {
                flag_UP = false;
                compasso++;
                if (compasso > 7) compasso = 1; 
                uartPutString("Compasso Incrementado\r\n");
                DISPLAY_CONVERSAO_COMPASSO(compasso);
            }
            if (flag_DOWN == true) {
                flag_DOWN = false;
                if (compasso <= 1) compasso = 7; 
                else compasso--;
                uartPutString("Compasso Decrementado\r\n");
                DISPLAY_CONVERSAO_COMPASSO(compasso);
            }
        }

        // ==========================================
        // TELA 2: MODO VISUAL
        // ==========================================
        if (tela_edicao == 2) {
            if (flag_UP == true) {
                flag_UP = false;
                modo_visual++;
                if (modo_visual > 3) modo_visual = 1;
                uartPutString("Modo Visual Incrementado\r\n");
                DISPLAY_TELA_VISUAL(modo_visual); 
            }
            if (flag_DOWN == true) {
                flag_DOWN = false;
                if (modo_visual <= 1) modo_visual = 3; 
                else modo_visual--;
                uartPutString("Modo Visual Decrementado\r\n");
                DISPLAY_TELA_VISUAL(modo_visual); 
            }
        }

        // ==========================================
        // BOTÃO START: Sair e Salvar
        // ==========================================
        if (flag_start == true) {
            flag_start = false;
            uartPutString("Saindo do Menu... Iniciando Metronomo\r\n");
            
            batida_atual = 1; 
            intervalo_ms = 60000 / bpm;
            tempo_ultimo_compasso = sistema_ms - intervalo_ms; 
            estado_metronomo = TOCANDO; 
            
            return; 
        }
    }
}