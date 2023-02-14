#include "startup.h"

uint32_t F_CPU;


void setup_SysClk(void)
{
    int iVar1;
    char cVar2;
    bool bVar3;
    stc_clk_freq_t stcClkFreq;
    stc_clk_sysclk_cfg_t stcSysClkCfg;
    en_clk_sys_source_t enSysClkSrc;
    stc_clk_mpll_cfg_t stcMpllCfg;
    stc_clk_xtal_cfg_t stcXtalCfg;

    memset(&stcSysClkCfg, 0, 7);
    memset(&stcXtalCfg, 0, 3);
    memset(&stcMpllCfg, 0, 0x14);
    stcSysClkCfg.enHclkDiv = ClkSysclkDiv1;
    stcSysClkCfg.enExclkDiv = ClkSysclkDiv2;
    stcSysClkCfg.enPclk0Div = ClkSysclkDiv1;
    stcSysClkCfg.enPclk1Div = ClkSysclkDiv2;
    stcSysClkCfg.enPclk2Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk3Div = ClkSysclkDiv4;
    stcSysClkCfg.enPclk4Div = ClkSysclkDiv2;
    CLK_SysClkConfig(&stcSysClkCfg);
    stcXtalCfg.enMode = ClkXtalModeOsc;
    stcXtalCfg.enDrv = ClkXtalLowDrv;
    stcXtalCfg.enFastStartup = Enable;
    CLK_XtalConfig(&stcXtalCfg);
    CLK_XtalCmd(en_functional_state::Enable);

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
    CLK_GetClockFreq(&stcClkFreq);
    F_CPU = stcClkFreq.pclk1Freq;
}

void getResetCause(void)
{
    //char *pcVar1;
    stc_rmu_rstcause_t stcRmuRstCause;

    memset(&stcRmuRstCause, 0, 0xf);
    RMU_GetResetCause(&stcRmuRstCause);
    if (stcRmuRstCause.enMultiRst == Set)
    {
        //pcVar1 = "[Multiply reset cause]->";
    }
    else
    {
        //pcVar1 = "";
    }
    //printf("ResetCause:%s", pcVar1);
    if (stcRmuRstCause.enXtalErr == Set)
    {
        //pcVar1 = "Xtal error reset";
    }
    else if (stcRmuRstCause.enClkFreqErr == Set)
    {
        //pcVar1 = "Clk freqence error reset";
    }
    else if (stcRmuRstCause.enRamEcc == Set)
    {
        //pcVar1 = "Ram ECC reset";
    }
    else if (stcRmuRstCause.enRamParityErr == Set)
    {
        //pcVar1 = "Ram parity error reset";
    }
    else if (stcRmuRstCause.enMpuErr == Set)
    {
        //pcVar1 = "Mpu error reset";
    }
    else if (stcRmuRstCause.enSoftware == Set)
    {
        //pcVar1 = "Software reset";
    }
    else if (stcRmuRstCause.enPowerDown == Set)
    {
        //pcVar1 = "Power down reset";
    }
    else if (stcRmuRstCause.enSwdt == Set)
    {
        //pcVar1 = "Special watchdog timer reset";
    }
    else if (stcRmuRstCause.enWdt == Set)
    {
        //pcVar1 = "Watchdog timer reset";
    }
    else if (stcRmuRstCause.enPvd2 == Set)
    {
        //pcVar1 = "Program voltage Dectection 2 reset";
    }
    else if (stcRmuRstCause.enPvd1 == Set)
    {
        //pcVar1 = "Program voltage Dectection 1 reset";
    }
    else if (stcRmuRstCause.enBrownOut == Set)
    {
        //pcVar1 = "Brown out reset";
    }
    else if (stcRmuRstCause.enRstPin == Set)
    {
        //pcVar1 = "Reset pin reset";
    }
    else if (stcRmuRstCause.enPowerOn == Set)
    {
        //pcVar1 = "Power on reset";
    }
    else
    {
        //pcVar1 = "unknowCause";
    }
    //printf("[%s]\n", pcVar1);
    return;
}

void init()
{
    //int iVar1;

    //_DAT_e000ed08 = 0xc000;
    setup_SysClk();
    setup_systick();
    setup_time();
    setup_gpio();
    setup_adcs();
    Ddl_UartInit();
    setup_Extinterrupt();
    getResetCause();
}
