#include <stdint.h>
#include "gpio.h"
#include "time.h"

/*
? GPIO_LEDS: 
* LED0 P8.3 GPIO1_6
* LED1 P8.5 GPIO1_7
* LED2 P8.7 GPIO1_2
* LED3 P8.9 GPIO1_3
* LED4 P8.11 GPIO2_2
? BUTTONS:
* BUTTON SET 
* BUTTON UP
* BUTTON DOWN
* BUTTON START
*/

int main(){
   uint8_t verif = 0;
   //liga os perifericos somente para o botão start

   //*Standby
   while (verif == 1){
      //fica preso no while ate o botão ser precionado
     
      //aqui ficara a chamada de sistema que atualizara a verif e sair desse 
      //loop incrementando o verif com ponteiro

   }
   
   //vai dar start nos modulos gpio e setas as primeiras variaveis
   //!start();

   /*
   uint32_t hora_da_proxima_batida = 0;
   uint32_t hora_de_desligar_o_pulso = 0;
   uint8_t pulso_ligado = 0; // Flag de estado: 0 = desligado, 1 = ligado
          
   // Matemática do tempo (Exemplo para 60 BPM = 1000ms de intervalo)
   uint32_t intervalo_bpm_ticks = 1000 * 24000; 
   uint32_t duracao_pulso_ticks = 20 * 24000; // O staccato de 20ms!
*/
   
   //*chamada de sistema obrigatoria para a primeira configuração do usuario de bpm;
   //*Saira somente quando setado uma configuração valida
   //!SET_BPM;

   while (1){
   //entramos no codigo 
   
   //*chamda de sistema voluntaria para set do bpm
   //!if(SET_BUTTON == 1) SET_BPM();

   }
   

   return 0;
}