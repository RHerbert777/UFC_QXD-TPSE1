#include "timer.h"
#include "metronomo.h"

#define WDT_BASE  0x44E35000
#define WDT_WSPR  0x48
#define WDT_WWPS  0x34

void WDT_Espera(uint32_t mascara) {
    while (HWREG(WDT_WWPS) & mascara);
}

void enable_wdt(uint32_t segundos) {
    uint32_t ciclos = segundos * 32768; // 32.768 Hz
    
    // Calcula de onde o contador deve começar para estourar no tempo certo
    uint32_t valor_carga = 0xFFFFFFFF - ciclos + 1;

    WDT_Espera(0x04); // Espera o WLDR liberar
    HWREG(WDT_WLDR) = valor_carga; //envia valor
    
    // 2. A Sequência de Partida (A senha BBBB e 4444)
    WDT_Espera(0x10); // Espera o WSPR liberar
    HWREG(WDT_WSPR) = 0xBBBB; //envia as duas outras senhas
    
    WDT_Espera(0x10); // Espera o WSPR liberar
    HWREG(WDT_WSPR) = 0x4444;
}

//função que não deixa o whatch dog resetar
void feed_wdt(void) {
    static uint32_t petisco = 0; // Guarda o último valor dado
    petisco++;
    
    WDT_Espera(0x08);    // Espera 
    HWREG(WDT_WTGR) = petisco;   // Entrega o petisco ao Cão!
}

void DESLIGAR_WATCHDOG_DE_FABRICA(void) {
    // 1ª Senha: Escreve 0xAAAA
    HWREG(WDT_BASE + WDT_WSPR) = 0xAAAA;
    
    // Espera o hardware confirmar a gravação
    while ((HWREG(WDT_BASE + WDT_WWPS) & (1 << 4)) != 0); 
    
    // 2ª Senha: Escreve 0x5555
    HWREG(WDT_BASE + WDT_WSPR) = 0x5555;
    
    // Espera o hardware confirmar a gravação final
    while ((HWREG(WDT_BASE + WDT_WWPS) & (1 << 4)) != 0); 
}

void ALIMENTAR_CAO_CONTROLADO(void) {
    // Usamos 'static' para a variável não morrer quando a função acabar,
    // guardando a memória de quando foi a última refeição.
    static uint32_t tempo_ultima_refeicao = 0; 

    // O cão só ganha comida se já passou 1 segundo (1000 ms) desde a última vez!
    if ((sistema_ms - tempo_ultima_refeicao) >= 1000) {
        
        feed_wdt(); // A chamada perigosa agora está enjaulada
        
        tempo_ultima_refeicao = sistema_ms; // Atualiza a hora da última refeição
    }
}

void timerSetup(void) {
    //PRCM
    HWREG(PRCM_TIMER7_CLKCTRL) |= 0x02; 
    while ((HWREG(PRCM_TIMER7_CLKCTRL) & 0x03) != 0x02);

    HWREG(DPLL_TIMER7_CLKSEL) = 0x01; //conecta ao cristal de 24mhz

    //posted libera a cpu para executar outras tarefas
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TSICR) |= (1 << 2);

    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 0));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCLR) &= ~(1 << 0);

    //zera o contador
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 1));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCRR) = 0;
    
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 2));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TLDR) = 0;
}

void mtimerDelay(uint32_t ms) {
    // 1 milissegundo = 24.000 ticks a 24 MHz
    uint32_t ticks_needed = ms * 24000;
    
    // Zera o contador
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 1));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCRR) = 0;
    
    // Dá o Start
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 0));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCLR) |= (1 << 0);
    
    // Fica lendo até atingir o tempo
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCRR) < ticks_needed);
    
    // Dá o Stop
    while (HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TWPS) & (1 << 0));
    HWREG(SOC_DMTIMER_7_REGS + DMTIMER_TCLR) &= ~(1 << 0);
}
//Uma ideia interesante é implementar timeout em codigos que podem entrar em loop um temp com um numero grande ele
//decrementando toda vez que entrar se ele decrementar muito e o codigo não tiver sido executado significa erro demorou
//muito tempo em no while retorna um erro 