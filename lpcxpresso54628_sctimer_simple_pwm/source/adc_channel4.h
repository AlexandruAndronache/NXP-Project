/*
 * adc_channel4.h
 *
 *  Created on: 26 Oct 2025
 *      Author: Alexandru Andronache
 */

#ifndef ADC_CHANNEL4_H_
#define ADC_CHANNEL4_H_

#include "fsl_adc.h"
#include <stdbool.h>
#include <stdint.h>

/*!
 * @brief Initializes ADC0 to read from channel 4 (P0_16 / J12.A0).
 *
 * Powers up the analog domain, performs calibration,
 * and enables interrupt-driven conversions.
 * P0_16 / ADC0_4 /  J12 PIN2
 */
void ADC_Channel4_Init(void);

/*!
 * @brief Reads one sample from ADC channel 4 (blocking mode).
 *
 * Starts a software-triggered conversion, waits for completion,
 * and returns a 12-bit raw ADC result (0–4095).
 *
 * @return Raw 12-bit ADC conversion result.
 */
uint32_t ADC_Channel4_Read(void);


/*
 * @brief Config the Channel
 * */

void ADC_Configuration(void);

/*!
 * @brief ADC interrupt handler for conversion sequence A.
 *
 * Handles interrupt flag, stores result, and signals conversion completion.
 */
void ADC0_SEQA_IRQHandler(void);

#endif /* ADC_CHANNEL4_H_ */
