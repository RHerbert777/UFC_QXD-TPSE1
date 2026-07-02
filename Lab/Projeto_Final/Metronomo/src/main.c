#include "metronomo.h"
//? "Nunca destranque a porta antes de prender os cachorros".
//? "Nunca alimente demais o cachoro senão ele morre de tanto comer"
//tftpboot 0x80000000 download.bin

/*
? GPIO_LEDS: 
? BLOCO coluna
* LED0 P8.3  GPIO1_6 / gpmc_ad6
* LED1 P8.5  GPIO1_2 / gpmc_ad2
* LED2 P8.16  GPIO1_14 / gpmc_ad14
* LED3 P8.26  GPIO1_29 / gpmc_csn0
* LED4 P8.12 GPIO1_12 / gpmc_ad12
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

//Tempos e compasos
volatile uint16_t bpm = 120;
volatile uint8_t  compasso = 4;
volatile uint8_t  batida_atual = 1;
volatile uint32_t intervalo_ms = 500;
volatile uint32_t tempo_ultimo_compasso = 0;
volatile uint32_t tempo_inicio_bipe = 0;

//Flags do Maestro 
volatile bool ordem_TOQUE = false;
volatile bool ordem_CORTE = false;
volatile bool som_esta_ativo = false;
volatile EstadoMetronomo estado_metronomo = STANDBY;

//flag modo visual
volatile uint8_t modo_visual = 1;

int main(){
   uartSetup();

   uartPutChar("=== Metronomo ===\r\n");

   start();

   //Modo de apresentação e testes
   Dispaly_test();
   LEDs_test();
   LED_SIMPLES_ON();

   uartPutChar("Esperando ordem para começar\r\n"); 
   //*Standby
   while (1){
      if(flag_start == true){
         flag_start = false;
         break;
      }
   }

   TM1637_Limpar_Tela();

   APAGAR_TODOS_LEDS_COLUNA();

   LED_SIMPLES_OFF();

   SET_BPM();

   while (1){
      //Interupção de set
      if(flag_SET == true){
         SET_BPM();
      }
      
      //START / STOP =======================================
      if (flag_start == true) {
         flag_start = false;
        
         if (estado_metronomo == STANDBY) {
            estado_metronomo = TOCANDO;
            tempo_ultimo_compasso = sistema_ms - intervalo_ms;//fazemos nossa contagem ira para o final
            batida_atual = 1; //reset da batida

         } else {
            estado_metronomo = STANDBY;
            ordem_CORTE = true; // O Maestro manda todo mundo calar a boca na hora do Pause
            uartPutString("<<< ORDEM: CORTE!\r\n");
         }
      }
      
      // MAESTRO - Gerador de eventos ========================
      if (estado_metronomo == TOCANDO) {
         
         //A diferença entre o tempo do sistema e do ultimo compaço tem que ser maior
         //que o intervalo do bpm para realizar um pulso
         if ((sistema_ms - tempo_ultimo_compasso) >= intervalo_ms) {
            tempo_ultimo_compasso = sistema_ms; // Reseta o cronômetro do compasso
            tempo_inicio_bipe = sistema_ms;     // Inicia o cronômetro do pulso sonoro
            
            som_esta_ativo = true;
            ordem_TOQUE    = true; // O Maestro levanta a bandeira de ataque!
         }
         
         // O momento exato de CORTAR (50ms depois do toque)
         if (som_esta_ativo == true) {
            uint32_t diferenca = sistema_ms - tempo_inicio_bipe;
          
            if (diferenca >= 50) { 
               som_esta_ativo = false;
               ordem_CORTE = true; 
            }
         }
      }

      //BUZZER
      if (ordem_TOQUE == true) {
         
         uartPutString("Lendo Batida: ");
         uartPutNumber(batida_atual);
         uartPutString("\r\n");
         
         if (batida_atual == 1){
            PWM_SET_FREQUENCY(440);
         }else{
            PWM_SET_FREQUENCY(220);
         }   
         
         PWM_PLAY();
      }
      if (ordem_CORTE == true) {
         PWM_STOP();
      }

      // MODO SIMPLES (1 ou 3)
      if (modo_visual == 1 || modo_visual == 3) {
         if (ordem_TOQUE == true) LED_SIMPLES_ON();
         if (ordem_CORTE == true) LED_SIMPLES_OFF();
      }
   
      // MODO PILHA ANIMADA (2 ou 3)
      if (modo_visual == 2 || modo_visual == 3) {
         if (estado_metronomo == TOCANDO) {
            Processar_Animacao_Coluna(); 
         } else {
            APAGAR_TODOS_LEDS_COLUNA();
         }
      }

      //Limpeza de flags e atualização de batidas
      if (ordem_TOQUE == true) {
         // Faz a contagem musical andar: 1 -> 2 -> 3 -> 4 -> 1...
         batida_atual++;
         if (batida_atual > compasso) {
            batida_atual = 1; 
         }
         ordem_TOQUE = false; // Desliga o sinal de rádio de início
      }
      
      if (ordem_CORTE == true) {
         ordem_CORTE = false; // Desliga o sinal de rádio de término
      }
   }
   return 0;
}