/* sample.c
 * Description:
 *
 * Author: Bryce Himebaugh
 * Contact: bhimebau@indiana.edu
 * Date: 03.18.2021
 * Copyright (C) 2021
 *
 */

#include <stdio.h>
#include <string.h>
#include "main.h"
#include "temp.h"
#include "battery.h"
#include "flash.h"

extern uint32_t period;
extern flash_status_t flash_status;
extern RTC_HandleTypeDef hrtc;
uint32_t time_encode(void);
void log_mem(char m[8]);
void time_decode(RTC_DateTypeDef *date, RTC_TimeTypeDef *time, uint32_t ts);
int read_log_records(flash_status_t *fs);

int sample_command(char *args) {
  if (args) {
    return 1;
  }
  else {
    printf("Sample Sensors\n\r");
    sample();
  }
  return 0;
}

int data_command(char *args) {
  if (args) {
    return 1;
  }
  else {
    read_data_records(&flash_status);
  }
  return 0;
}

int log_command(char *args){
	  if (args) {
	    return 1;
	  }
	  else {
	    read_log_records(&flash_status);
	  }
	  return 0;
}

int l_command(char *args){
	char msg[8];
	if(args){
		memcpy(msg, args, 8);
	}else{
		strcpy(msg, "command");
	}
	log_mem(msg);
	return 0;
}


int sample(void) {
  sensordata_t sd;

  sd.watermark = 1;
  sd.status=1;
  sd.battery_voltage = (uint16_t) read_vrefint();
  sd.temperature = (uint16_t) read_temp();
  sd.sensor_period = period;
  sd.timestamp = 0;
  sd.timestamp = time_encode();
  write_raw(&flash_status,(raw_t *) &sd);
  return(0);
}

void log_mem(char m[8]){
	logdata_t d;
	d.watermark = 1;
	d.status = 2;
	d.timestamp = 0;
	d.timestamp = time_encode();
	memcpy(d.msg, m, 8);
	write_raw(&flash_status,(raw_t *) &d);
}



uint32_t time_encode(void){
	RTC_DateTypeDef date;
	RTC_TimeTypeDef time;
	HAL_RTC_GetTime(&hrtc,&time,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN);
	uint32_t ts = 0;
	ts = ((uint32_t)date.Month<<28) | ((uint32_t)date.Date<<23) | ((uint32_t)date.Year<<17) |((uint32_t)time.Hours<<12) | ((uint32_t)time.Minutes<<6) | ((uint32_t)time.Seconds);
	//printf("%lx", ts);
	return ts;
}

void time_decode(RTC_DateTypeDef *date, RTC_TimeTypeDef *time, uint32_t ts){
	time->Seconds = (uint8_t) (ts&0x3f);
	time->Minutes =  (uint8_t) ((ts>>6)&0x3f);
	time->Hours = (uint8_t) ((ts>>12)&0x1f);
	date->Year = (uint8_t) ((ts>>17)&0x3f);
	date->Date = (uint8_t) ((ts>>23)&0x1f);
	date->Month = (uint8_t) ((ts>>28)&0xf);
	//printf("\r\n%2.2u/%2.2u/%4.4u %2.2u:%2.2u:%2.2u\r\n",date->Month,date->Date,date->Year+2000,time->Hours,time->Minutes,time->Seconds);
}
int read_data_records(flash_status_t *fs) {
  RTC_DateTypeDef date;
  RTC_TimeTypeDef time;

  sensordata_t * p;
  int count = 0;
  p = (sensordata_t *) fs->data_start;
  printf("\r\n");
  while (p>((sensordata_t *)fs->next_address)) {
    if (p->status==1) {
      time_decode(&date, &time, p->timestamp);
      printf("D,%d,%2.2u/%2.2u/%4.4u,%2.2u:%2.2u:%2.2u,%d.%d,%d,%d\n\r",
             count,
			 date.Month,date.Date,date.Year+2000,time.Hours,time.Minutes,time.Seconds,
             p->battery_voltage/1000,
             p->battery_voltage%1000,
             p->temperature,
             (int) p->sensor_period);
      count++;
    }
    p--;
  }
  return(0);
}

int read_log_records(flash_status_t *fs) {
  RTC_DateTypeDef date;
  RTC_TimeTypeDef time;

  logdata_t * p;
  int count = 0;
  p = (logdata_t *) fs->data_start;

  while (p>((logdata_t *)fs->next_address)) {
    if (p->status==2) {
      time_decode(&date, &time, p->timestamp);
      printf("\r\nL,%d,%2.2u/%2.2u/%4.4u,%2.2u:%2.2u:%2.2u,%s\n\r",
             count,
			 date.Month,date.Date,date.Year+2000,time.Hours,time.Minutes,time.Seconds,
      	  	  p->msg);
      count++;
    }
    p--;
  }
  return(0);
}
