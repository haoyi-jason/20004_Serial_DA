/*
 * app_20004.c
 *
 *  Created on: 2020¦~5¤ë30¤é
 *      Author: Jason
 */

#include "ch.h"
#include "hal.h"
#include "at24_eep.h"
#include "app_20004.h"

static const _nvm_param_t  default_nvm_param = {
  "EP01",
  "MODULE PARAM",
  {{100,1000},
  {100,1000},
  {100,1000}}
};

static _app_param_t appParam;

void app_2004_do_write(uint8_t ch, uint8_t v);
thread_t *self;

static PWMConfig pwmcfg = {
  100000,                                    /* 10kHz PWM clock frequency.   */
  100,                                    /* Initial PWM period 0.01S.       */
  NULL,
  {
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL},
   {PWM_OUTPUT_ACTIVE_HIGH, NULL}
  },
  0,
  0,
};

static SerialConfig serialCfg={
  115200
};

#define MSG_BUF_SZ      32
#define MSG_BUF_MASK    MSG_BUF_SZ-1
#define CMD_PACKET_SIZE 13

#define QUERY_PACKET_SZ 9
#define NOF_QUERY   2
static char query_str[2][9]={
 {0x2,0x30,0x31,0x52,0x50,0x41,0x34,0x34,0x03},
 {0x2,0x30,0x31,0x52,0x50,0x42,0x34,0x35,0x03}
};

enum query_state_e{
  QUERY_IDLE,
  QUERY_CHA,
  QUERY_CHB
};

//#define AO_TO_DUTY(x)   (x*100)

uint16_t AO_TO_DUTY(uint8_t ch,float engVal)
{
  uint16_t pwm = 0;
  if(engVal < appParam.nvm.da_config[ch].engLow) pwm = appParam.nvm.da_config[ch].zeroValue;
  else if(engVal > appParam.nvm.da_config[ch].engHigh) pwm = appParam.nvm.da_config[ch].fullValue;
  else{
    uint16_t span = appParam.nvm.da_config[ch].fullValue - appParam.nvm.da_config[ch].zeroValue+1;
    float ratio = (engVal - appParam.nvm.da_config[ch].engLow)/(appParam.nvm.da_config[ch].engHigh - appParam.nvm.da_config[ch].engLow);
    pwm = appParam.nvm.da_config[ch].zeroValue + (uint16_t)(ratio*span);
  }
  return pwm;
}

static THD_WORKING_AREA(waCmdParser, 512);
static THD_FUNCTION(procCmdParse, arg) {

  (void)arg;
  chRegSetThreadName("CMD_PARSER");
  eventflags_t flags;
  char msgBuf[128];
  uint16_t duty = 0;
  sdStart(&SD1,&serialCfg);
  uint8_t rxIndex,wrIndex;
  event_listener_t elSerialData;
  chEvtRegisterMask((event_source_t*)chnGetEventSource(&SD1),&elSerialData,EVENT_MASK(1));
  rxIndex = wrIndex = 0;
  bool validPacket = false;
  uint8_t query_target = QUERY_IDLE;
  uint8_t waitResp = 0;
  for(uint8_t i=0;i<3;i++){
    appParam.runtime.duty[i] = 0;
  }
  uint8_t test = 0;
  while (true) {
    chEvtWaitOneTimeout(EVENT_MASK(1),TIME_MS2I(10));
    chSysLock();
    flags = chEvtGetAndClearFlags(&elSerialData);
    chSysUnlock();
    if(flags & CHN_INPUT_AVAILABLE){
      msg_t charBuf;
      do{
        charBuf = chnGetTimeout(&SD1,TIME_IMMEDIATE);
        if(charBuf != Q_TIMEOUT){
          if(charBuf == 0x02){
            wrIndex = 0;
          }else if(charBuf == 0x03){
            validPacket = true;
          }else{
            msgBuf[(wrIndex++) & MSG_BUF_MASK] = (char)charBuf;
          }
        }
      }while(charBuf != Q_TIMEOUT);

      // buffer valid
      if(validPacket){
        validPacket = false;
        if(strncmp(msgBuf,"01rP",4)==0){
          float v = 0;
          v = (msgBuf[6]-0x30)*10+(msgBuf[7]-0x30);
          v += (msgBuf[8]-0x30)*0.1;
          //v += (msgBuf[10]-0x30)*0.01;
//          v += (msgBuf[11]-0x30)*0.1;
          if(msgBuf[5] == 'F')
            v *= -1;

          switch(msgBuf[4]){
          case 'A':
            query_target = QUERY_CHB;
            // update pwm
            pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, AO_TO_DUTY(0,v)));
            break;
          case 'B':
            query_target = QUERY_CHA;
            pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, AO_TO_DUTY(1,v)));
            break;
          }
        }
        else if(strncmp(msgBuf,"02w",3)==0){ // write param 02wxy, x: nof config, y: start index
          char tmp[8];
          uint16_t v1,v2;
          uint8_t nofCfg = msgBuf[3] - 0x30;
          uint8_t start = msgBuf[4] - 0x30;
          for(uint8_t i=0;i<nofCfg;i++){
            memcpy(tmp,msgBuf[5+i*10],5);
            sscanf(tmp,"%d",&v1);
            memcpy(tmp,msgBuf[5+(i+1)*10],5);
            sscanf(tmp,"%d",&v2);
            appParam.nvm.da_config[i].zeroValue = v1;
            appParam.nvm.da_config[i].fullValue = v2;
          }
        }
        else if(strncmp(msgBuf,"02r",3)==0){ // read param

        }
      }
    } // end serial input available
    if(waitResp == 0){
      query_target++;
      if(query_target == NOF_QUERY) query_target = 0;
      chnWriteTimeout(&SD1,query_str[query_target],QUERY_PACKET_SZ,TIME_MS2I(100));
      waitResp = 10;
    }else{
      waitResp--;
    }
    if(test == 1){
      pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, duty));
      pwmEnableChannel(&PWMD2, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, duty));
      pwmEnableChannel(&PWMD2, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, duty));
      duty += 500;
      if(duty > 10000) duty = 0;
    }

    chThdSleepMilliseconds(500);
  }
}

void save_param(void)
{
  eepromWrite(0x0,sizeof(_nvm_param_t),&appParam.nvm);
}

void load_param(void)
{
  eepromRead(0x0,sizeof(_nvm_param_t),&appParam.nvm);
  if(strncmp(EEP_FLAG,appParam.nvm.flag,4) != 0){
    memcpy(&appParam.nvm,&default_nvm_param,sizeof(_nvm_param_t));
    save_param();
  }
}

void app_2004_init(void)
{
  pwmStart(&PWMD2,&pwmcfg);
  pwmEnableChannel(&PWMD2, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 0));
  pwmEnableChannel(&PWMD2, 1, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 0));
  pwmEnableChannel(&PWMD2, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD2, 0));
  at24eep_init(&I2CD1,32,1024,0x50,2);
  load_param();
  self = chThdCreateStatic(waCmdParser, sizeof(waCmdParser), NORMALPRIO, procCmdParse, NULL);

}


void app_2004_do_write(uint8_t ch, uint8_t v)
{

}
