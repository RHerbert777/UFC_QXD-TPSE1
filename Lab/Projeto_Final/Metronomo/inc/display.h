#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdint.h>

#define TM1637_CLK_PIN 19  // GPIO1_19 (P9_16)
#define TM1637_DIO_PIN 17  // GPIO1_17 (P9_23)

// ==========================================
// CONSTANTES DO DISPLAY
// ==========================================
#define TM1637_CMD_FIXED_ADDR 0x44  // Comando: Escrever em um endereço específico
#define TM1637_CMD_DISPLAY_ON 0x8F  // Comando: Ligar tela com brilho máximo (88 a 8F)

#define TM1637_POS_1 0xC0  // Endereço do 1º dígito (Esquerda)
#define TM1637_POS_2 0xC1  // Endereço do 2º dígito
#define TM1637_POS_3 0xC2  // Endereço do 3º dígito
#define TM1637_POS_4 0xC3  // Endereço do 4º dígito (Direita)

// Inicializa os pinos GPIO e liga a tela do display
void TM1637_Init(void);

// Escreve um número (0 a 9) em uma posição específica (TM1637_POS_1 a POS_4)
void TM1637_Escrever_Digito(uint8_t posicao, uint8_t numero);

// Limpa toda a tela (apaga os 4 dígitos)
void TM1637_Limpar_Tela(void);

void TM1637_Escrever_Segmentos_Brutos(uint8_t posicao, uint8_t dados_brutos) ;
#endif
