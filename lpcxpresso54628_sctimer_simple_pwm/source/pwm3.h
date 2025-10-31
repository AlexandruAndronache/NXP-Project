/*
 * pwm3.h
 *
 *  Created on: 25 oct. 2025
 *      Author: alexa
 */

#ifndef PWM3_H_
#define PWM3_H_

#include <stdint.h>

/*!
 * @brief Discrete allowed duty cycles (0% ... 100%)
 *
 * You can pass either these enum values or any 0..100 integer
 * into PWM_UpdateDuty().
 */
typedef enum
{
    duty_0_percent   = 0U,
    duty_10_percent  = 10U,
    duty_20_percent  = 20U,
    duty_30_percent  = 30U,
    duty_40_percent  = 40U,
    duty_50_percent  = 50U,
    duty_60_percent  = 60U,
    duty_70_percent  = 70U,
    duty_80_percent  = 80U,
    duty_90_percent  = 90U,
    duty_100_percent = 100U
} duty_cycle_increments_t;

/*!
 * @brief Initialize board HW and configure/start the SCTIMER PWM.
 *
 * This will:
 *  - init board clocks/pins/console
 *  - init SCTIMER
 *  - configure PWM on kSCTIMER_Out_3 (P3_10 / J13-7 on LPCXpresso54628)
 *  - set initial duty cycle to 0%
 *  - set frequency to 24 kHz
 *  - start the timer
 *
 * All configuration (output pin, frequency, PWM mode) is fixed internally.
 *
 * @return 0 on success, -1 on failure
 */
int PWM_InitAndStart(void);

/*!
 * @brief Update duty cycle (no re-init).
 *
 * The channel, event, and timer context are remembered internally.
 *
 * @param duty_pct New duty in percent [0..100]
 */
void PWM_UpdateDuty(uint8_t duty_pct);

#endif /* PWM3_H_ */
