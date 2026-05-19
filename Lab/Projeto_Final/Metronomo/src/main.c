#include "Func.h"
/*
* PIN 11 GPIO1_13 (OUTPUT)
* PIN 12 GPIO1_15 (INPUT)
* PIN 20 GPIO1_31 (INPUT - OUTPUT)
* PIN 21 GPIO1_30 (INPUT - OUTPUT)
* PIN 27 GPIO2_22 (INPUT - OUTPUT)
* PIN 28 GPIO2_24 (INPUT - OUTPUT)
* PIN 39 GPIO2_12 (INPUT - OUTPUT)
* PIN 40 GPIO2_13 (INPUT - OUTPUT)
*
*
*/



int main(){
   //Aguardando botão start
    while(1){
        if(start() =! 0) exit();
   }

   //inicialização do sistema testando todos as funcionalidades
   Inicialização(void);

   //Principal
   while (1){
    
   }
   


}