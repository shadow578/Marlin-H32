#include "startup.h"

uint32_t CPU_FREQ;
stc_rmu_rstcause_t RESET_CAUSE;

void setup_SysClk()
{
    int iVar1;
    char cVar2;
    bool bVar3;

    stc_clk_sysclk_cfg_t stcSysClkCfg;
    en_clk_sys_source_t enSysClkSrc;

    // configure system clock dividers
    stc_clk_sysclk_cfg_t stcSysClkCfg;
    stcSysClkCfg.enHclkDiv = ClkSysclkDiv1;
    stcSysClkCfg.enExclkDiv = ClkSysclkDiv2;
    stcSysClkCfg.enPclk0Div = ClkSysclkDiv1;
    stcSysClkCfg.enPclk1Div = ClkSysclkDiv2;
    stcSysClkCfg.enPclk2Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk3Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk4Div = ClkSysclkDiv2;
    CLK_SysClkConfig(&stcSysClkCfg);

    // configure and enable XTAL oscillator
    stc_clk_xtal_cfg_t stcXtalCfg;
    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv = ClkXtalLowDrv;
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(en_functional_state::Enable);

    // configure MPLL with XTAL as clock source
    stc_clk_mpll_cfg_t stcMpllCfg;
    stcMpllCfg.pllmDiv = 1;
    stcMpllCfg.plln = 0x32;
    stcMpllCfg.PllpDiv = 4;
    stcMpllCfg.PllqDiv = 4;
    stcMpllCfg.PllrDiv = 4;
    CLK_SetPllSource(en_clk_pll_source::ClkPllSrcXTAL);

    CLK_MpllConfig(&stcMpllCfg);
    EFM_Unlock();
    EFM_SetLatency(5);
    EFM_InstructionCacheCmd(en_functional_state::Enable);
    EFM_Lock();
    CLK_MpllCmd(en_functional_state::Enable);

    do
    {
        iVar1 = CLK_GetFlagStatus(en_clk_flag::ClkFlagMPLLRdy);
        bVar3 = iVar1 != 1;
        if (bVar3)
        {
            iVar1 = 1;
        }
        cVar2 = (char)iVar1;
        if (!bVar3)
        {
            cVar2 = '\0';
        }
    } while (cVar2 != '\0');

    CLK_SetSysClkSource(en_clk_sys_source::CLKSysSrcMPLL);
    CLK_GetSysClkSource();

    // get clock frequency and write it to CPU_FREQ
    stc_clk_freq_t stcClkFreq;
    CLK_GetClockFreq(&stcClkFreq);
    CPU_FREQ = stcClkFreq.pclk1Freq;
}

void init()
{
    // int iVar1;

    //_DAT_e000ed08 = 0xc000;
    setup_SysClk();
    setup_systick();
    setup_time();
    setup_gpio();
    setup_adcs();
    Ddl_UartInit();
    setup_Extinterrupt();

    // get cause of last reset
    RMU_GetResetCause(&RESET_CAUSE);
}
