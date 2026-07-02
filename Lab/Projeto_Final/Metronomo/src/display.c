#include "display.h"
#include "gpio.h"
#include "soc_AM335x.h"

//Pinos definidos DIO e CLK
#define CLK_PIN CM_conf_gpmc_a3
#define DIO_PIN CM_conf_gpmc_a1

// TABELA DE TRADUÇÃO (Números de 0 a 9)
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

//Micro delay por garantia
void delay_micros() {
    for(volatile int i = 0; i < 500; i++); 
}

//Inicializa o modulo TM1637 com clk e dio definidos como alto
void TM1637_Init(void) {
    MUX_CONFIG(0x7, CLK_PIN); 
    
    MUX_CONFIG(0x27, DIO_PIN); 

    // DIREÇÃO INICIAL (Ambos começam como SAÍDA)
    CONF_DIR(SOC_GPIO_1_REGS, TM1637_CLK_PIN, GPIO_OUTPUT);
    CONF_DIR(SOC_GPIO_1_REGS, TM1637_DIO_PIN, GPIO_OUTPUT);

    // 3. ESTADO DE REPOUSO (Ambos em 1 / 3.3V)
    GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN);
    GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_DIO_PIN);

    delay_micros();
    TM1637_DisplayOn(7);
}

// A regra de START do protocolo: 
// Com o CLK em ALTO, o pino DIO sofre uma queda para BAIXO.
void TM1637_Start(void) {
    
    GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN);
    GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_DIO_PIN);
    delay_micros();
    
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, TM1637_DIO_PIN); // DIO cai primeiro
    delay_micros();
    
    GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN); // CLK cai depois
    delay_micros();
}


// A regra de STOP do protocolo:
// Com o CLK em ALTO, o pino DIO sofre uma subida para ALTO.
void TM1637_Stop(void) {
    
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
        
        // Abaixa o Relógio (Prepara para trocar o valor do fio de dados)
        GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN);
        delay_micros();
        
        // Coloca 0 ou 1 no DIO, dependendo do último bit da variável 'dado'
        if (dado & 0x01) {
            GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_DIO_PIN);
        } else {
            GPIO_CLEAN_PIN(SOC_GPIO_1_REGS, TM1637_DIO_PIN);
        }
        delay_micros();
        
        // Sobe o Relógio (Dá o "tique" pro display ler o fio)
        GPIO_SET_PIN(SOC_GPIO_1_REGS, TM1637_CLK_PIN);
        delay_micros();
        
        // Empurra a variável para a direita para expor o próximo bit na próxima volta
        dado = dado >> 1; 
    }
    
    // 9º PULSO: Lendo o ACK (Acknowledge)
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

//Escreve segmentos brutos de dados
void TM1637_Escrever_Segmentos_Brutos(uint8_t posicao, uint8_t dados_brutos) {
    //Configura o display para modo de "Endereço Fixo"
    TM1637_Start();
    TM1637_Escrever_Byte(0x44); 
    TM1637_Stop();

    // Envio do Dado Bruto para a posição específica
    TM1637_Start();
    TM1637_Escrever_Byte(0xC0 + posicao); 
    TM1637_Escrever_Byte(dados_brutos);   
    TM1637_Stop();
}

// ESCREVER UM NÚMERO NA TELA (0 a 9)
void TM1637_Escrever_Digito(uint8_t posicao, uint8_t numero) {
    uint8_t codigo_segmento;

    // Proteção: Se pedir um número maior que 9, apaga o dígito
    if (numero <= 9) {
        codigo_segmento = codigo_numeros[numero];
    } else {
        codigo_segmento = 0x00; // 0x00 significa todos os LEDs apagados
    }

    //Configuração de Endereço Fixo
    TM1637_Start();
    TM1637_Escrever_Byte(TM1637_CMD_FIXED_ADDR);
    TM1637_Stop();

    //Endereço do Dígito + O Valor
    TM1637_Start();
    TM1637_Escrever_Byte(posicao);
    TM1637_Escrever_Byte(codigo_segmento);
    TM1637_Stop();

    //Ligar Tela e Ajustar Brilho
    TM1637_DisplayOn(7);
}

// APAGAR TELA COMPLETA
void TM1637_Limpar_Tela(void) {
    //Envia o comando de Escrita de Dados com auto-incremento (0x40)
    TM1637_Start();
    TM1637_Escrever_Byte(0x40); 
    TM1637_Stop();

    // Define o endereço inicial apontando para o 1º dígito (0xC0)
    TM1637_Start();
    TM1637_Escrever_Byte(0xC0); 
    
    //Envia o "Vazio" (0x00) sequencialmente para as 4 posições
    TM1637_Escrever_Byte(0x00); // Apaga o 1º dígito
    TM1637_Escrever_Byte(0x00); // Apaga o 2º dígito
    TM1637_Escrever_Byte(0x00); // Apaga o 3º dígito
    TM1637_Escrever_Byte(0x00); // Apaga o 4º dígito
    
    TM1637_Stop();
}

//Desliga completamente o display
void TM1637_DisplayOff(void) {
    TM1637_Start();
    // O comando 0x80 desliga a tela (0x88 a 0x8F liga a tela definindo o brilho)
    TM1637_Escrever_Byte(0x80); 
    TM1637_Stop();
}

// Liga o display e define o brilho (0 = Mínimo, 7 = Máximo)
void TM1637_DisplayOn(uint8_t brilho) {
    // Trava de segurança: garante que o brilho não passe de 7
    if (brilho > 7) brilho = 7; 
    
    TM1637_Start();
    // O comando base para ligar é 0x88. Somando o brilho, vamos de 0x88 a 0x8F
    TM1637_Escrever_Byte(0x88 + brilho); 
    TM1637_Stop();
}

//Logica para display invertido =======================================================

// Função que gira qualquer número ou letra do TM1637 de cabeça para baixo
uint8_t TM1637_Inverter_Segmentos(uint8_t segmentos) {
    uint8_t invertido = 0;
    
    if (segmentos & 0x01) invertido |= 0x08; // Segmento A vira D
    if (segmentos & 0x02) invertido |= 0x10; // Segmento B vira E
    if (segmentos & 0x04) invertido |= 0x20; // Segmento C vira F
    if (segmentos & 0x08) invertido |= 0x01; // Segmento D vira A
    if (segmentos & 0x10) invertido |= 0x02; // Segmento E vira B
    if (segmentos & 0x20) invertido |= 0x04; // Segmento F vira C
    if (segmentos & 0x40) invertido |= 0x40; // Segmento G (Meio) continua G
    
    return invertido;
}

void TM1637_Escrever_Digito_Invertido(uint8_t posicao_logica, uint8_t valor) {
    uint8_t desenho_original = codigo_numeros[valor];
    uint8_t desenho_invertido = TM1637_Inverter_Segmentos(desenho_original);

    //Inverte o Eixo X: O que deveria ir para a direita, vai para a esquerda
    uint8_t posicao_fisica;
    if (posicao_logica      == TM1637_POS_1) posicao_fisica = TM1637_POS_4;
    else if (posicao_logica == TM1637_POS_2) posicao_fisica = TM1637_POS_3;
    else if (posicao_logica == TM1637_POS_3) posicao_fisica = TM1637_POS_2;
    else if (posicao_logica == TM1637_POS_4) posicao_fisica = TM1637_POS_1;
    else posicao_fisica = posicao_logica;

    // Comunica com o hardware usando os comandos originais
    TM1637_Start();
    TM1637_Escrever_Byte(posicao_fisica);
    TM1637_Escrever_Byte(desenho_invertido);
    TM1637_Stop();
}

void TM1637_Escrever_Segmentos_Brutos_Invertido(uint8_t posicao, uint8_t segmentos_brutos) {
    //Inverte o eixo Y (Gira o desenho 180 graus)
    uint8_t segmentos_corrigidos = TM1637_Inverter_Segmentos(segmentos_brutos);

    //Inverte o eixo X (Tradução da Posição Lógica para Física)
    
    uint8_t posicao_fisica = 3 - posicao; 

    //Envia para o hardware usando a sua função original
    TM1637_Escrever_Segmentos_Brutos(posicao_fisica, segmentos_corrigidos);
}