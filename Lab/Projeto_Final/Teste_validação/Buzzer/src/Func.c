#include "BBB_REGS.h"
#include "Func.h"
#include <stdint.h>

//Desabilita o whach dog timmer 
void disable_wdt(void) {
    unsigned int addr_wspr = SOC_WDT_1_REGS + WDT_WSPR;
    unsigned int addr_wwps = SOC_WDT_1_REGS + WDT_WWPS;

    HWREG(addr_wspr) = 0xAAAA;

    while ( (HWREG(addr_wwps) & (1 << 4)) != 0 );

    HWREG(addr_wspr) = 0x5555;

    while ( (HWREG(addr_wwps) & (1 << 4)) != 0 );
}
//Ativa o modulo PCRM de um modulo com o offser colocando 2 enable
void PRCM_ENABLE_MODE(uint32_t module_offset) {
    unsigned int addr_prcm = SOC_CM_PER_REGS + module_offset;
    
    //Aplica 0x2 para colocar modulo como enable
    HWREG(addr_prcm) |= 0x2; 
}

void SET_BUZZER(){
    //SYSCONFIG -------------------------------------------------------------
    unsigned int addr_temp = SOC_PWMSS1_REGS + PWMSS1_SYSCONFIG;
    unsigned int val_temp = HWREG(addr_temp);
    
    val_temp &= ~0x3E; //limpando os 5 bits que iremos configurar

    val_temp |= (1 << 4); // STANDBYMODE = 1h (No-standby)
    val_temp |= (1 << 2); // IDLEMODE = 1h (No-idle)
    val_temp |= (1 << 1); // FREEEMU = 1h (Free Emulation)

    HWREG(addr_temp) = val_temp;

    //SYSCLOCK ----------------------------------------------------------------
    addr_temp = SOC_PWMSS1_REGS + PWMSS1_CLKCONFIG;
    val_temp = HWREG(addr_temp);

    //ligando o modulo ePWMCLK_EN
    val_temp |= (1<<8);
    
    HWREG(addr_temp) = val_temp;

    //SYSSTATUS ---------------------------------------------------------------
    addr_temp = SOC_PWMSS1_REGS + PWMSS1_CLKSTATUS; // Offset 0x0C
    while ( (HWREG(addr_temp) & (1 << 8)) == 0 ) ;
    // Aguarda o Bit 8 virar 1 (Ack do hardware)
    
    // 1. Configura a regra da onda: 
    // Sobe para 3.3V quando zerar, cai para 0V quando empatar com o CMPA
    HWREG_16(SOC_EHRPWM1_REGS + AQCTLA) = 0x0012;

    // 2. Configura o contador para o modo Crescente (Up-Count)
    HWREG_16(SOC_EHRPWM1_REGS + TBCTL) = 0x0000;
    
    // 3. Inicia o sistema em completo silêncio
    PWM_STOP();

}

void PWM_SET_FREQUENCY(uint32_t freq_hz){
    // Trava de segurança para não explodir a matemática (divisão por zero)
    if (freq_hz == 0) return;

    // A CPU calcula o teto exato do contador para a nota pedida
    uint32_t valor_tbprd = PWM_CLOCK_HZ / freq_hz;
    
    // Grava o período no registrador de 16 bits
    HWREG_16(SOC_EHRPWM1_REGS + TBPRD) = (unsigned short)valor_tbprd;
}

void PWM_PLAY() {
    // O pulo do gato: A função lê o período atual e divide por 2.
    // Assim, o Duty Cycle fica SEMPRE cravado em exatos 50%, 
    // não importa qual nota (frequência) você esteja tocando!
    unsigned short periodo_atual = HWREG_16(SOC_EHRPWM1_REGS + TBPRD);
    HWREG_16(SOC_EHRPWM1_REGS + CMPA) = periodo_atual / 2;
}

void PWM_STOP() {
    // Zera a largura do pulso (Duty Cycle = 0%)
    HWREG_16(SOC_EHRPWM1_REGS + CMPA) = 0;
}