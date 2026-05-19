#include "bbb_regs.h"

static void delay() {
  volatile unsigned int ra;
  for(ra = 0; ra < 1000000; ra ++);
}

void putCh ( unsigned char c ) {
  while(!(UART0_LSR & (1<<5)));
  UART0_THR = c;
}/* -----  end of function putCh  ----- */

unsigned int printS ( unsigned char *str, unsigned int length ) {
  for(int i = 0; i < length; i++)
    putCh(str[i]);
  return(length);
}/* -----  end of function printS  ----- */

int main(){
  const char *hello = "Hello World!!!\n\r";
  unsigned char *h = (unsigned char *)hello;
  while(1) {
    printS(h, 16);
    delay();
  }
}/* -----  end of function main  ----- */
