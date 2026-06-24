#include "interupt.h"
#include "timer.h"

extern volatile uint8_t flag_botao_1;
extern volatile uint8_t flag_botao_2;
extern volatile uint8_t flag_timer; // Flag de software do Timer

void IntAINTCInit(void) {
    HWREG(SOC_AINTC_REGS + INTC_SYSCONFIG) = INTC_SYSCONFIG_SOFTRESET;
    
    while((HWREG(SOC_AINTC_REGS + INTC_SYSSTATUS) & INTC_SYSSTATUS_RESETDONE) != INTC_SYSSTATUS_RESETDONE);
    
    HWREG(SOC_AINTC_REGS + INTC_THRESHOLD) = INTC_THRESHOLD_PRIORITYTHRESHOLD;
}

//Função que seta a interupção para um modulo especifico
//Recebe modulo, pino e o gatilho borda de subida ou descida
void gpioInterruptSetup(uint32_t modulo_gpio, uint8_t pino, uint8_t tipo_gatilho) {
    //habilitando
    HWREG(modulo_gpio + GPIO_IRQSTATUS_SET_0) |= (1 << pino);

    //zerando antigas
    HWREG(modulo_gpio + GPIO_RISINGDETECT)  &= ~(1 << pino);
    HWREG(modulo_gpio + GPIO_FALLINGDETECT) &= ~(1 << pino);

    // Tipo de borda
    if (tipo_gatilho == EDGE_RISING || tipo_gatilho == EDGE_BOTH) {
        HWREG(modulo_gpio + GPIO_RISINGDETECT) |= (1 << pino);
    }
    if (tipo_gatilho == EDGE_FALLING || tipo_gatilho == EDGE_BOTH) {
        HWREG(modulo_gpio + GPIO_FALLINGDETECT) |= (1 << pino);
    }

    // qual o modulo 
    if (modulo_gpio == SOC_GPIO_0_REGS) {
        // GPIO0 é a interrupção 96 -> MIR_CLEAR3 (Bit 0)
        HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR3) = (1 << 0); 
    }
    else if (modulo_gpio == SOC_GPIO_1_REGS) {
        // GPIO1 é a interrupção 98 -> MIR_CLEAR3 (Bit 2)
        HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR3) = (1 << 2); 
    }
    else if (modulo_gpio == SOC_GPIO_2_REGS) {
        // GPIO2 é a interrupção 32 -> MIR_CLEAR1 (Bit 0)
        HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR1) = (1 << 0); 
    }
    else if (modulo_gpio == SOC_GPIO_3_REGS) {
        // GPIO3 é a interrupção 62 -> MIR_CLEAR1 (Bit 30)
        HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR1) = (1 << 30); 
    }
}

void timerinteruptsetup(uint32_t modulo_timer){
    if (modulo_timer == SOC_DMTIMER_7_REGS) {
        HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR2) = (1 << 31);
    }
    else if (modulo_timer == SOC_DMTIMER_2_REGS) { // SOC_DMTIMER_2_REGS
        HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR2) = (1 << 4);
    }
    else if (modulo_timer == SOC_DMTIMER_3_REGS) { // SOC_DMTIMER_3_REGS
        HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR2) = (1 << 5);
    }
}

void delay_ms_interrupt(unsigned int mSec) {
    unsigned int countVal = 0xFFFFFFFF - (mSec * 24000);

    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCRR) = countVal;

    flag_timer = false;//abaixa a flag

    // O valor 0x2 habilita a interrupção por Overflow (Estouro)
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_IRQENABLE_SET) = 0x2;

    // Dá o Start no Timer
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCLR) |= 0x1;

    while (flag_timer == false) {
        //cpu espera
    }

    // desabilitando interupção
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_IRQENABLE_CLR) = 0x2;
    
    //zerando o start
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCLR) &= ~0x1;
}

void ISR_Handler(){
    unsigned int intrNum = HWREG(SOC_AINTC_REGS + INTC_SIR_IRQ) & 0x7F;

    if (intrNum == 98) { //if gpio1
        // Tira uma foto para saber quais pinos do GPIO1 estão com a flag física levantada
        uint32_t status_gpio1 = HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_0);

        // Foi o Botão 1 (Pino 16)?
        if (status_gpio1 & (1 << 16)) {
            flag_botao_1 = true;
            HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_0) = (1 << 16); // Limpa a evidência física
        }
        
        // Foi o Botão 2 (Pino 28)?
        if (status_gpio1 & (1 << 28)) {
            flag_botao_2 = true;
            HWREG(SOC_GPIO_1_REGS + GPIO_IRQSTATUS_0) = (1 << 28); 
        }

    }else if(intrNum == 32){ //if gpio2
        //trabalho final
    }else if(intrNum == 62){ //if gpio 3
        //trabalho final
    }else if (intrNum == 95){ //timer 7
        HWREG(SOC_DMTIMER_7_REGS + 0x28) = 0x2;
        flag_timer = true;
        HWREG(SOC_AINTC_REGS + INTC_MIR_CLEAR2) = (1 << 31);
    }
    
    //Desligando interupção
    HWREG(SOC_AINTC_REGS + INTC_CONTROL) = 0x1;
}