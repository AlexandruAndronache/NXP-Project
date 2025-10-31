/*
 * pwm3.c
 *
 *  Created on: 25 oct. 2025
 *      Author: alexa
 */

#include "pwm3.h"
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"
#include "fsl_sctimer.h"

/* These are internal-only. Nobody else needs to see them. */
static uint32_t pwmEventId = 0U;

/* Fixed configuration values */
/* P3_10 / J13-7 PIN CONFIG*/
static const sctimer_out_t pwm_out       = kSCTIMER_Out_3;          /* P3_10 / J13-7 */
static const uint32_t      pwm_freq_hz   = 24000U;                  /* 24 kHz */
static const uint8_t       init_duty_pct = 0U;                      /* start at 0% */
static const sctimer_pwm_mode_t pwm_mode = kSCTIMER_EdgeAlignedPwm; /* edge-aligned */

int PWM_InitAndStart(void)
{
    sctimer_config_t sctimerInfo;
    sctimer_pwm_signal_param_t pwmParam;
    uint32_t sctimerClock;

    /* Init board (pins, clocks, UART, etc.) */
    BOARD_InitHardware();

    /* Timer clock source in Hz */
    sctimerClock = CLOCK_GetFreq(kCLOCK_BusClk);

    /* Load default config and init SCT0 */
    SCTIMER_GetDefaultConfig(&sctimerInfo);
    SCTIMER_Init(SCT0, &sctimerInfo);

    /* PWM channel config */
    pwmParam.output           = pwm_out;
    pwmParam.level            = kSCTIMER_HighTrue;  /* active-high pulses */
    pwmParam.dutyCyclePercent = init_duty_pct;

    /* Configure PWM */
    if (SCTIMER_SetupPwm(SCT0,
                         &pwmParam,
                         pwm_mode,
                         pwm_freq_hz,
                         sctimerClock,
                         &pwmEventId) == kStatus_Fail)
    {
        PRINTF("\r\nSCTIMER_SetupPwm FAIL");
        return -1;
    }

    /* Start timer in unified counter mode */
    SCTIMER_StartTimer(SCT0, kSCTIMER_Counter_U);

    PRINTF("\r\nPWM started on SCT out %u at %lu Hz, duty %u%%",
           (uint32_t)pwm_out,
           (unsigned long)pwm_freq_hz,
           (uint32_t)init_duty_pct);

    return 0;
}

void PWM_UpdateDuty(uint8_t duty_pct)
{
    /* clamp just in case caller gives >100 */
    if (duty_pct > 100U)
    {
        duty_pct = 100U;
    }

    SCTIMER_UpdatePwmDutycycle(SCT0, pwm_out, duty_pct, pwmEventId);

    PRINTF("\r\nPWM duty updated to %u%%", (uint32_t)duty_pct);
}
