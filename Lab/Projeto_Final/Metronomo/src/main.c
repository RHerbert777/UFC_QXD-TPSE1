#include "metronomo.h"
//? "Nunca destranque a porta antes de prender os cachorros".
//? "Nunca alimente demais o cachoro senão ele morre de tanto comer"
//tftpboot 0x80000000 download.bin
/*
? GPIO_LEDS: 
* LED0 P8.3  GPIO1_6 / gpmp_ad6
* LED1 P8.5  GPIO1_7 / gpmp_ad2
* LED2 P8.7  GPIO1_2 / gpmp_advn_ale
* LED3 P8.9  GPIO1_3 / gpmp_b0n_cle
* LED4 P8.11 GPIO2_2 / gpmp_ad13

? BUTTONS:
! BUTTON SET   P8.18 gpio2_1  gpmc_clk_mux0
! BUTTON UP    P8.10 gpio2_4  gpmc_wen
! BUTTON DOWN  P8.20 gpio1_31 gpmc_csn2
! BUTTON START P8.24 gpio1_1  gpmc_ad1

? BUZZER:
* P9.14 

? DISPLAY
* CLK P9.16 gpmc_a3
* DIO P9.23 gpmc_a1
*/

/*
! IDEIAS 
* Precisamos separar os dois blocos fundamentais em bloco led bloco  
*
*/
volatile uint16_t bpm = 120;
volatile uint8_t compasso = 4;
volatile uint8_t batida_atual = 1;
volatile uint32_t intervalo_ms = 500;

// 2. Relógios do Sistema
volatile uint32_t tempo_ultimo_compasso = 0;
volatile uint32_t tempo_inicio_bipe = 0;
// (A variável 'sistema_ms' provavelmente nasce dentro do seu timer.c)

// 3. Flags do Maestro 
volatile bool ordem_TOQUE = false;
volatile bool ordem_CORTE = false;
volatile bool som_esta_ativo = false;

volatile EstadoMetronomo estado_metronomo = STANDBY;
volatile uint8_t modo_visual = 1;

int main(){
   uartSetup();

   start();

   uartPutString("uart - start\r\n");

   //*Standby
   while (1){
      if(flag_start == true){
         flag_start = false;
         break;
      }
      //ALIMENTAR_CAO_CONTROLADO();
      flag_start = true;
   }
   
   uartPutString("inicio start\r\n");
   SET_BPM();
   

   while (1){
      //feed_wdt();

      //Bloco do maestro, a cpu entrará nesse bloco para verificar o estado das variaveis e se foi ou não apertado
      //a interupção de set
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
         }
      }
      
      // (Gerador de Eventos)============================================================
      if (estado_metronomo == TOCANDO) {
        
         //A diferença entre o tempo do sistema e do ultimo compaço tem que ser maior
         //que o intervalo do bpm para realizar um pulso
         if ((sistema_ms - tempo_ultimo_compasso) >= intervalo_ms) {
            tempo_ultimo_compasso = sistema_ms; // Reseta o cronômetro do compasso
            tempo_inicio_bipe = sistema_ms;     // Inicia o cronômetro do pulso sonoro
            
            som_esta_ativo = true;
            ordem_TOQUE = true; // O Maestro levanta a bandeira de ataque!
         }
        
         // O momento exato de CORTAR (50ms depois do toque)
         if (som_esta_ativo == true) {
            if ((sistema_ms - tempo_inicio_bipe) >= 50) {
               som_esta_ativo = false;
                
               ordem_CORTE = true; // O Maestro levanta a bandeira de silêncio!
            }
         }
      }

      //BUZZER
      if (ordem_TOQUE == true) {
        if (batida_atual == 1) PWM_SET_FREQUENCY(1000); 
        else PWM_SET_FREQUENCY(500);  
      }
      if (ordem_CORTE == true) {
        PWM_STOP();          
      }

      if (ordem_CORTE == true) {
         ordem_CORTE = false; // Consome a ordem
         PWM_STOP();          // Corta a energia do periférico
      }
      
      // MODO SIMPLES (Agora é 1 ou 3)
      if (modo_visual == 1 || modo_visual == 3) {
        if (ordem_TOQUE == true) LED_SIMPLES_ON();
        if (ordem_CORTE == true) LED_SIMPLES_OFF();
      }

      // MODO PILHA ANIMADA (Agora é 2 ou 3)
      if (modo_visual == 2 || modo_visual == 3) {
         if (estado_metronomo == TOCANDO) {
            Processar_Animacao_Coluna(); 
         } else {
            APAGAR_TODOS_LEDS_COLUNA();
         }
      }

      // ==========================================================
      // BLOCO 4: O MAESTRO LIMPA A MESA
      // ==========================================================
      // Depois que todo mundo reagiu às bordas, o Maestro encerra o evento.
      ordem_TOQUE = false;
      ordem_CORTE = false;
   }
   return 0;
}