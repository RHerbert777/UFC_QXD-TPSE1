#include "display.h"
#include "gpio.h"
#include "soc_AM335x.h"
// ========================================================
// TABELA DE TRADUÇÃO (Números de 0 a 9)
// ========================================================
const uint8_t codigo_numeros[] = {
    0x3F, // 0
    0x06, // 1
    0x5B, // 2
    0x4F, // 3
    0x66, // 4
    0x6D, // 5
    0x7D, // 6
    0x07, // 7
    0x7F, // 8
    0x6F  // 9
};

//Pinos de DIO e CLK
#define CLK_PIN CM_conf_gpmc_a3    // P9_16 = GPIO1_19 (GPMC_A3) - CLK
#define DIO_PIN CM_conf_gpmc_a1    // P9_23 = GPIO1_17 (GPMC_A1) - DIO

void delay_micros() {
    for(volatile int i = 0; i < 500; i++); 
}

void TM1637_Init(void) {
    // Atenção ao DIO (P9_23): Como ele é bidirecional e o display vai 
    // tentar "falar" de volta com a BeagleBone, precisamos do ouvido ligado!
    // Lembra do nosso vilão de hoje mais cedo? RX_ACTIVE ativado (0x27)!
    
    MUX_CONFIG(0x7, CLK_PIN); 
    
    MUX_CONFIG(0x27, DIO_PIN); 

    // DIREÇÃO INICIAL (Ambos começam como SAÍDA)
    CONF_DIR(SOC_GPIO_1_REGS, TM1637_CLK_PIN, GPIO_OUTPUT);
    CONF_DIR(SOC_GPIO_1_REGS, TM1637_DIO_PIN, GPIO_OUTPUT);

    // 3. ESTADO DE REPOUSO (Ambos em 1 / 3.3V)
    GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN);
    GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_DIO_PIN);
}

void TM1637_Start(void) {
    // A regra de START do protocolo: 
    // Com o CLK em ALTO, o pino DIO sofre uma queda para BAIXO.
    
    GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN);
    GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_DIO_PIN);
    delay_micros();
    
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, TM1637_DIO_PIN); // DIO cai primeiro
    delay_micros();
    
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN); // CLK cai depois
    delay_micros();
}


void TM1637_Stop(void) {
    // A regra de STOP do protocolo:
    // Com o CLK em ALTO, o pino DIO sofre uma subida para ALTO.
    
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN);
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, TM1637_DIO_PIN);
    delay_micros();
    
    GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN); // CLK sobe primeiro
    delay_micros();
    
    GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_DIO_PIN); // DIO sobe depois, fechando o pacote
    delay_micros();
}

// Função interna para enviar 8 bits (1 Byte) para o display
void TM1637_Escrever_Byte(uint8_t dado) {
    // Laço para enviar os 8 bits, do menos significativo (LSB) para o mais (MSB)
    for (int i = 0; i < 8; i++) {
        
        // 1. Abaixa o Relógio (Prepara para trocar o valor do fio de dados)
        GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN);
        delay_micros();
        
        // 2. Coloca 0 ou 1 no DIO, dependendo do último bit da variável 'dado'
        if (dado & 0x01) {
            GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_DIO_PIN);
        } else {
            GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, TM1637_DIO_PIN);
        }
        delay_micros();
        
        // 3. Sobe o Relógio (Dá o "tique" pro display ler o fio)
        GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN);
        delay_micros();
        
        // 4. Empurra a variável para a direita para expor o próximo bit na próxima volta
        dado = dado >> 1; 
    }
    
    // ==========================================
    // O 9º PULSO: Lendo o ACK (Acknowledge)
    // ==========================================
    // Abaixa o clock
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN);
    
    // Transforma o nosso pino DIO temporariamente em ENTRADA para ouvir o display
    CONF_DIR(SOC_GPIO_1_REGS, TM1637_DIO_PIN, GPIO_INPUT);
    delay_micros();
    
    // Sobe o clock (O display joga o DIO para GND neste momento se entendeu a mensagem)
    GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN);
    delay_micros();
    
    // Abaixa o clock para finalizar o ciclo do ACK
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN);
    
    // Transforma o DIO de volta em SAÍDA para a BeagleBone voltar a comandar
    CONF_DIR(SOC_GPIO_1_REGS, TM1637_DIO_PIN, GPIO_OUTPUT);
    delay_micros();
}

// ========================================================
// ESCREVER UM NÚMERO NA TELA (0 a 9)
// ========================================================
void TM1637_Escrever_Digito(uint8_t posicao, uint8_t numero) {
    uint8_t codigo_segmento;

    // Proteção: Se pedir um número maior que 9, apaga o dígito
    if (numero <= 9) {
        codigo_segmento = codigo_numeros[numero];
    } else {
        codigo_segmento = 0x00; // 0x00 significa todos os LEDs apagados
    }

    // 1º CARTA: Configuração de Endereço Fixo
    TM1637_Start();
    TM1637_Escrever_Byte(TM1637_CMD_FIXED_ADDR);
    TM1637_Stop();

    // 2º CARTA: Endereço do Dígito + O Valor (Ex: 0xC0 e o valor 0x06 para o '1')
    TM1637_Start();
    TM1637_Escrever_Byte(posicao);
    TM1637_Escrever_Byte(codigo_segmento);
    TM1637_Stop();

    // 3º CARTA: Ligar Tela e Ajustar Brilho
    TM1637_Start();
    TM1637_Escrever_Byte(TM1637_CMD_DISPLAY_ON);
    TM1637_Stop();
}

// ========================================================
// APAGAR TELA COMPLETA
// ========================================================
void TM1637_Limpar_Tela(void) {
    // Manda um "número falso" (10) para todos os dígitos apagarem
    TM1637_Escrever_Digito(TM1637_POS_1, 10);
    TM1637_Escrever_Digito(TM1637_POS_2, 10);
    TM1637_Escrever_Digito(TM1637_POS_3, 10);
    TM1637_Escrever_Digito(TM1637_POS_4, 10);
}