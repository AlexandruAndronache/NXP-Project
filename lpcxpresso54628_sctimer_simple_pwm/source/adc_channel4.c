/*
 * adc_channel4.c
 *
 *  Created on: 25 oct. 2025
 *      Author: alexa
 */


/*
 * adc_channel4.c
 *
 *  Created on: 26 Oct 2025
 *      Author: Alexandru Andronache
 */

#include "adc_channel4.h"
#include "fsl_clock.h"
#include "fsl_power.h"
#include "fsl_iocon.h"
#include "fsl_debug_console.h"

/* ---------------- Configuration constants ---------------- */

#define DEMO_ADC_BASE                  ADC0
#define DEMO_ADC_SAMPLE_CHANNEL_NUMBER 4U
#define DEMO_ADC_IRQ_ID                ADC0_SEQA_IRQn
#define DEMO_ADC_CLOCK_DIVIDER         2U
#define DEMO_ADC_CLOCK_MODE            kADC_ClockSynchronousMode

/* ---------------- Private variables ---------------- */

 adc_result_info_t gAdcResultInfoStruct;
 volatile bool gAdcConvSeqAIntFlag = false;
 const uint32_t gAdcFullRange = 4096U;

/* ---------------- Private function prototypes ---------------- */
void ADC_ClockPower_Configuration(void);
void ADC_Configuration(void);
void ADC_InitPin_P0_16(void);

/* ---------------- Implementation ---------------- */

 void ADC_ClockPower_Configuration(void)
{
    POWER_DisablePD(kPDRUNCFG_PD_VDDA);
    POWER_DisablePD(kPDRUNCFG_PD_ADC0);
    POWER_DisablePD(kPDRUNCFG_PD_VD2_ANA);
    POWER_DisablePD(kPDRUNCFG_PD_VREFP);
    POWER_DisablePD(kPDRUNCFG_PD_TS);

    CLOCK_EnableClock(kCLOCK_Adc0);
}

 void ADC_InitPin_P0_16(void)
{
    CLOCK_EnableClock(kCLOCK_Iocon);

    IOCON->PIO[0][16] =
        ((IOCON->PIO[0][16] &
          (~(IOCON_PIO_FUNC_MASK | IOCON_PIO_MODE_MASK | IOCON_PIO_DIGIMODE_MASK))) |
         IOCON_PIO_FUNC(0U) |                   /* FUNC0 = ADC0_4 */
         IOCON_PIO_MODE(IOCON_MODE_INACT) |     /* No pull-up/down */
         IOCON_PIO_DIGIMODE(0U));               /* Analog mode */
}

void ADC_Channel4_Init(void)
{
    adc_config_t adcConfig;
    adc_conv_seq_config_t seqConfig;

    /* Enable ADC power and clocks */
    ADC_ClockPower_Configuration();

    /* Configure the pin */
    ADC_InitPin_P0_16();

    /* Initialize ADC peripheral */
    ADC_GetDefaultConfig(&adcConfig);
#if defined(FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE) && FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE
    adcConfig.clockMode = DEMO_ADC_CLOCK_MODE;
#endif
    adcConfig.clockDividerNumber = DEMO_ADC_CLOCK_DIVIDER;
#if defined(FSL_FEATURE_ADC_HAS_CTRL_RESOL) && FSL_FEATURE_ADC_HAS_CTRL_RESOL
    adcConfig.resolution = kADC_Resolution12bit;
#endif
#if defined(FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE) && FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE
    adcConfig.enableLowPowerMode = false;
#endif
#if defined(FSL_FEATURE_ADC_HAS_TRIM_REG) && FSL_FEATURE_ADC_HAS_TRIM_REG
    adcConfig.voltageRange = kADC_HighVoltageRange;
#endif

    ADC_Init(DEMO_ADC_BASE, &adcConfig);

    /* Calibrate the ADC */
#if !(defined(FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC) && FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC)
    uint32_t freq = CLOCK_GetFreq(kCLOCK_BusClk);
    if (ADC_DoOffsetCalibration(DEMO_ADC_BASE, freq))
    {
        PRINTF("ADC calibration successful.\r\n");
    }
    else
    {
        PRINTF("ADC calibration failed.\r\n");
    }
#endif

    /* Configure Sequence A */
    memset(&seqConfig, 0, sizeof(seqConfig));
    seqConfig.channelMask      = (1U << DEMO_ADC_SAMPLE_CHANNEL_NUMBER);
    seqConfig.triggerMask      = 0U;  /* software trigger only */
    seqConfig.triggerPolarity  = kADC_TriggerPolarityPositiveEdge;
    seqConfig.enableSingleStep = false;
    seqConfig.enableSyncBypass = false;
    seqConfig.interruptMode    = kADC_InterruptForEachSequence;

    ADC_SetConvSeqAConfig(DEMO_ADC_BASE, &seqConfig);
    ADC_EnableConvSeqA(DEMO_ADC_BASE, true);

    /* Enable interrupts */
    ADC_EnableInterrupts(DEMO_ADC_BASE, kADC_ConvSeqAInterruptEnable);
    NVIC_EnableIRQ(DEMO_ADC_IRQ_ID);

    PRINTF("ADC channel 4 initialized.\r\n");
}

/* ---------------- ADC read and ISR ---------------- */

uint32_t ADC_Channel4_Read(void)
{
    gAdcConvSeqAIntFlag = false;

    ADC_DoSoftwareTriggerConvSeqA(DEMO_ADC_BASE);

    return gAdcResultInfoStruct.result;
}



void ADC_Configuration(void)
{
    adc_config_t adcConfigStruct;
    adc_conv_seq_config_t adcConvSeqConfigStruct;

/* Configure the converter. */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE) & FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE
    adcConfigStruct.clockMode = DEMO_ADC_CLOCK_MODE;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_ASYNMODE */
    adcConfigStruct.clockDividerNumber = DEMO_ADC_CLOCK_DIVIDER;
#if defined(FSL_FEATURE_ADC_HAS_CTRL_RESOL) & FSL_FEATURE_ADC_HAS_CTRL_RESOL
    adcConfigStruct.resolution = kADC_Resolution12bit;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_RESOL */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL) & FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL
    adcConfigStruct.enableBypassCalibration = false;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_BYPASSCAL */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_TSAMP) & FSL_FEATURE_ADC_HAS_CTRL_TSAMP
    adcConfigStruct.sampleTimeNumber = 0U;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_TSAMP */
#if (defined(FSL_FEATURE_ADC_HAS_GPADC_CTRL0_GPADC_TSAMP) && FSL_FEATURE_ADC_HAS_GPADC_CTRL0_GPADC_TSAMP)
    adcConfigStruct.extendSampleTimeNumber = kADC_ExtendSampleTimeNotUsed;
#endif /* FSL_FEATURE_ADC_HAS_GPADC_CTRL0_GPADC_TSAMP */
#if defined(FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE) & FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE
    adcConfigStruct.enableLowPowerMode = false;
#endif /* FSL_FEATURE_ADC_HAS_CTRL_LPWRMODE */
#if defined(FSL_FEATURE_ADC_HAS_TRIM_REG) & FSL_FEATURE_ADC_HAS_TRIM_REG
    adcConfigStruct.voltageRange = kADC_HighVoltageRange;
#endif /* FSL_FEATURE_ADC_HAS_TRIM_REG */
    ADC_Init(DEMO_ADC_BASE, &adcConfigStruct);

#if !(defined(FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC) && FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC)
    uint32_t frequency = 0U;
    /* Calibration after power up. */
#if defined(FSL_FEATURE_ADC_HAS_CALIB_REG) && FSL_FEATURE_ADC_HAS_CALIB_REG
    DEMO_ADC_BASE->CTRL |= ADC_CTRL_BYPASSCAL_MASK;
    frequency = CLOCK_GetFreq(kCLOCK_BusClk);
    if (true == ADC_DoOffsetCalibration(DEMO_ADC_BASE, frequency))
#else
#if defined(SYSCON_ADCCLKDIV_DIV_MASK)
    frequency = CLOCK_GetFreq(DEMO_ADC_CLOCK_SOURCE) / CLOCK_GetClkDivider(kCLOCK_DivAdcClk);
#else
    frequency = CLOCK_GetFreq(DEMO_ADC_CLOCK_SOURCE);
#endif /* SYSCON_ADCCLKDIV_DIV_MASK */
    if (true == ADC_DoSelfCalibration(DEMO_ADC_BASE, frequency))
#endif /* FSL_FEATURE_ADC_HAS_CALIB_REG */
    {
        PRINTF("ADC Calibration Done.\r\n");
    }
    else
    {
        PRINTF("ADC Calibration Failed.\r\n");
    }
#if defined(FSL_FEATURE_ADC_CALIBRATION_CLOCK_LOWER_THAN_30MHz) && FSL_FEATURE_ADC_CALIBRATION_CLOCK_LOWER_THAN_30MHz
    ReInitSystemclock();
#endif
#endif /* FSL_FEATURE_ADC_HAS_NO_CALIB_FUNC */


    /* Enable channel DEMO_ADC_SAMPLE_CHANNEL_NUMBER's conversion in Sequence A. */
#if defined(FSL_FEATURE_ADC_HAS_SEQ_CTRL_TSAMP) & FSL_FEATURE_ADC_HAS_SEQ_CTRL_TSAMP
    adcConvSeqConfigStruct.seqSampleTimeNumber = 0U;
#endif /* FSL_FEATURE_ADC_HAS_SEQ_CTRL_TSAMP */
    adcConvSeqConfigStruct.channelMask =
        (1U << DEMO_ADC_SAMPLE_CHANNEL_NUMBER); /* Includes channel DEMO_ADC_SAMPLE_CHANNEL_NUMBER. */
    adcConvSeqConfigStruct.triggerMask      = 0U;
    adcConvSeqConfigStruct.triggerPolarity  = kADC_TriggerPolarityPositiveEdge;
    adcConvSeqConfigStruct.enableSingleStep = false;
    adcConvSeqConfigStruct.enableSyncBypass = false;
    adcConvSeqConfigStruct.interruptMode    = kADC_InterruptForEachSequence;
    ADC_SetConvSeqAConfig(DEMO_ADC_BASE, &adcConvSeqConfigStruct);
    ADC_EnableConvSeqA(DEMO_ADC_BASE, true); /* Enable the conversion sequence A. */
    /* Clear the result register. */
    ADC_DoSoftwareTriggerConvSeqA(DEMO_ADC_BASE);
    while (!ADC_GetChannelConversionResult(DEMO_ADC_BASE, DEMO_ADC_SAMPLE_CHANNEL_NUMBER, &gAdcResultInfoStruct))
    {
    }
    ADC_GetConvSeqAGlobalConversionResult(DEMO_ADC_BASE, &gAdcResultInfoStruct);
}

void ADC0_SEQA_IRQHandler(void)
{
    if (ADC_GetStatusFlags(DEMO_ADC_BASE))
    {
     while(!ADC_GetChannelConversionResult(DEMO_ADC_BASE,
                                       DEMO_ADC_SAMPLE_CHANNEL_NUMBER,
                                       &gAdcResultInfoStruct));

        ADC_ClearStatusFlags(DEMO_ADC_BASE, kADC_ConvSeqAInterruptFlag);
        gAdcConvSeqAIntFlag = true;
    }
    SDK_ISR_EXIT_BARRIER;
}
