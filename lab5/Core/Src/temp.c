/* temperature.c
 * Description: STM32L432KC Temperture Read Function
 *
 * Author: Bryce Himebaugh
 * Contact: bhimebau@indiana.edu
 * Date: 04.20.2019
 * Copyright (C) 2019
 *
 */

#include "main.h"
#include <string.h>
#include <stdio.h>
#include "temp.h"
#include "battery.h"

extern ADC_HandleTypeDef hadc1;


float read_temp(void) {
  static uint32_t first_time_up = 1;
  static ADC_ChannelConfTypeDef sConfig = {0};
  uint32_t rawTemp;
  uint32_t vref;
  float temp;


  if (first_time_up) {
    sConfig.Channel = ADC_CHANNEL_TEMPSENSOR;
    sConfig.Rank = 1;
    sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
    sConfig.SingleDiff = ADC_SINGLE_ENDED;
    sConfig.OffsetNumber = ADC_OFFSET_NONE;
    sConfig.Offset = 0;
    first_time_up = 0;
  }
  sConfig.Rank = 1;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
      Error_Handler();
  }
  while (HAL_ADCEx_Calibration_Start(&hadc1,ADC_SINGLE_ENDED) != HAL_OK);   // Calibrate the A2D
  HAL_ADC_Start(&hadc1);
  HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);
  rawTemp = HAL_ADC_GetValue(&hadc1);
  HAL_ADC_Stop(&hadc1);
  vref = read_vrefint();
  return (__HAL_ADC_CALC_TEMPERATURE(vref,
                                       rawTemp,
                                       ADC_RESOLUTION_12B));
  return (temp);
}
