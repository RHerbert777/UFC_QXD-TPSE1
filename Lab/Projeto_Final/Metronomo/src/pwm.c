#include "pwm.h"
#include "gpio.h"

//Ativa o modulo PCRM de um modulo com o offser colocando 2 enable
void PRCM_ENABLE_MODE(uint32_t module_offset) {
    unsigned int addr_prcm = SOC_CM_PER_REGS + module_offset;

    HWREG(addr_prcm) |= 0x2; 

    while ((HWREG(addr_prcm) & 0x30000) != 0);
    return;
}

//Setamos o PWM pasando o modulo do pwm que será configurado
void SET_BUZZER(uint32_t module_offset){
    unsigned int addr_prcm = SOC_CM_PER_REGS + module_offset;

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

    HWREG(SOC_CONTROL_REGS + PWMSS_CTRL) |= (1 << 1);//Liberando a trava

    // Configura a regra da onda: 
    // Sobe para 3.3V quando zerar, cai para 0V quando empatar com o CMPA
    HWREGH(SOC_EHRPWM1_REGS + AQCTLA) = 0x0012;

    // Configura o contador para o modo Crescente (Up-Count)
    HWREGH(SOC_EHRPWM1_REGS + TBCTL) = 0x0000;
    
    // Inicia o sistema em completo silêncio
    PWM_STOP();
}
//seta a frequencia em hz 
void PWM_SET_FREQUENCY(uint32_t freq_hz){
    //evita divisão por zero
    if (freq_hz == 0) return;

    // A CPU calcula o teto com 32 bits livremente
    uint32_t valor_tbprd = PWM_CLOCK_HZ / freq_hz;
    
    HWREGH(SOC_EHRPWM1_REGS + TBPRD) = (unsigned short)valor_tbprd;

    return;
}

void PWM_PLAY() {
    unsigned short periodo_atual = HWREGH(SOC_EHRPWM1_REGS + TBPRD);
    HWREGH(SOC_EHRPWM1_REGS + CMPA) = periodo_atual / 2;
    return;
}

void PWM_STOP() {
    // Zera a largura do pulso (Duty Cycle = 0%)
    HWREGH(SOC_EHRPWM1_REGS + CMPA) = 0;
    return;
}