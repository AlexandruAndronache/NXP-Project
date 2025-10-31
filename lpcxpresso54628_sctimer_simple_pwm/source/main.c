/*
 * main.c
 *
 *  Created on: 28 oct. 2025
 *      Author: alexa
 */
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_sctimer.h"
#include "fsl_debug_console.h"
#include "board.h"
#include "app.h"
#include "fsl_sctimer.h"
#include "adc_channel4.h"
#include "stdio.h"

#define DEMO_ADC_BASE                  ADC0
#define DEMO_ADC_SAMPLE_CHANNEL_NUMBER 4U
#define DEMO_ADC_IRQ_ID                ADC0_SEQA_IRQn
#define DEMO_ADC_IRQ_HANDLER_FUNC      ADC0_SEQA_IRQHandler
#define DEMO_ADC_CLOCK_DIVIDER         2U
#define PWM_DUTY_FACTOR 0.244
#define DEMO_ADC_CLOCK_MODE kADC_ClockSynchronousMode

 adc_result_info_t gAdcResultInfoStruct1  = {0};
extern bool gAdcConvSeqAIntFlag;


void ADC_ClockPower_Configuration();
void ADC_InitPin_P0_16();

enum e_duty
{
	duty_0_percent = 0,
	duty_10_percent = 10,
	duty_20_percent = 20,
	duty_30_percent = 30,
	duty_40_percent = 40,
	duty_50_percent = 50,
	duty_60_percent = 60,
	duty_70_percent = 70,
	duty_80_percent = 80,
	duty_90_percent = 90,
	duty_100_percent = 100,
};


int main (void)
{


    sctimer_config_t sctimerInfo;
    sctimer_pwm_signal_param_t pwmParam;
    uint32_t event;
    uint32_t sctimerClock;
    BOARD_InitHardware();
    ADC_ClockPower_Configuration();
    ADC_Configuration();
    ADC_InitPin_P0_16();

    // HW
    sctimerClock = SCTIMER_CLK_FREQ;

    // Default values for PWM
    SCTIMER_GetDefaultConfig(&sctimerInfo);

    /* Initialize SCTimer module */
    SCTIMER_Init(SCT0, &sctimerInfo);

    /* Configure first PWM with frequency 24kHZ from first output */
    // P3_10 - J13 - 7
    pwmParam.output           = kSCTIMER_Out_3;
    pwmParam.level            = kSCTIMER_HighTrue;
    pwmParam.dutyCyclePercent = duty_0_percent;
    uint32_t PWM_signal_frequency_Hz = 20000U;

    // Setup PWM and guard for fail
    if (SCTIMER_SetupPwm(SCT0, &pwmParam, kSCTIMER_EdgeAlignedPwm, PWM_signal_frequency_Hz, sctimerClock, &event) == kStatus_Fail)
    {
    	PRINTF("\r\nSCTIMER_SetupPwm FAIL");
        return -1;
    }

    // USE THIS TO UPDATE THE DUTY CYCLE
    uint8_t new_duty_cycle = duty_50_percent;
    PRINTF("\r\nPWM at %d", new_duty_cycle);
    SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_3, new_duty_cycle, event);
    PRINTF("\r\nFan Duty updated to %d",  (uint32_t)new_duty_cycle);


    int duty = 0;
    while (1)
    {

//    	 SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_3, duty++, event);
//    	 if (duty == 100) duty = 0;
//    	 PRINTF("\r\nFan Duty updated to %d",  (uint32_t)duty);

    	 ADC_DoSoftwareTriggerConvSeqA(DEMO_ADC_BASE);
		 while(!ADC_GetChannelConversionResult(DEMO_ADC_BASE,DEMO_ADC_SAMPLE_CHANNEL_NUMBER,&gAdcResultInfoStruct1))
		 {

		 }

		 /* redraw */
		 PRINTF("\033[2J\033[H");
		 PRINTF("\rgADC        = %d", gAdcResultInfoStruct1.result);

		 if (PWM_DUTY_FACTOR * gAdcResultInfoStruct1.result > 100)
		 {
			 new_duty_cycle = 100;
		 }
		 else
		 {
			 new_duty_cycle = PWM_DUTY_FACTOR * gAdcResultInfoStruct1.result;
		 }

		 SCTIMER_UpdatePwmDutycycle(SCT0, kSCTIMER_Out_3, new_duty_cycle, event);
		 fflush(stdout);

		 SDK_DelayAtLeastUs(200000U, CLOCK_GetFreq(kCLOCK_CoreSysClk)); // 200 ms







    }
}


