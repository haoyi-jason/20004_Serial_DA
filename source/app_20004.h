/*
 * app_20004.h
 *
 *  Created on: 2020¦~5¤ë30¤é
 *      Author: Jason
 */

#ifndef SOURCE_APP_20004_H_
#define SOURCE_APP_20004_H_

#define NOF_DA_CH   3
#define NOF_DO_CH   2
#define EEP_FLAG    "EP01"
struct da_config{
  uint16_t zeroValue; // pwm output for 4mA
  uint16_t fullValue; // pwm output for 20mA
  float engLow; // engineering value for 4mA output
  float engHigh;// engineering value for 20mA output
};

struct do_config{
  uint8_t logic; // 0: normal, 1: reverse
  uint8_t temp[15]; // reserve for later use
};

typedef struct{
  uint8_t flag[4];
  uint8_t moduleInfo[64];
  struct da_config da_config[NOF_DA_CH];
  struct do_config do_config[NOF_DO_CH];
}_nvm_param_t;

typedef struct{
  uint32_t runSecs;
  uint16_t duty[NOF_DA_CH];
}_rumtime_param_t;

typedef struct{
  _nvm_param_t nvm;
  _rumtime_param_t runtime;
}_app_param_t;


void app_2004_init(void);

#endif /* SOURCE_APP_20004_H_ */
