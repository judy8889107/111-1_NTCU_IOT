/*
 * Copyright (c) 2013, Marcus Linderoth, http://forfunandprof.it
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

/**
 * \file
 *         Simple-PWM example application,
 *          Demonstrates using the simple_pwm API to eg dim LED(閃爍LED).
 * \author
 *         Marcus Linderoth <linderoth.marcus@gmail.com>
 */

#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/button.h"
#include "simple-pwm.h"

/* pins I want to use for PWM (only one is used) at a time with this */
/* 定義腳位 */
#define LEDRED_PIN       (0)   // P1.0
#define LEDGRN_PIN       (6)   // P1.6
#define LEDBIG_PIN       (7)   // P1.7 開發板上沒有

/* the PWM duty cycle will step back and forth between these limits, with this step */

#define PWM_MIN           0   /* 最低百分之0 */
#define PWM_MAX           100 /* 最高百分之百 */
#define PWM_STEP          1   /* 每次step增加1，這樣就要跑100個迴圈才能跑到100 */
/* 漸暗或漸亮 */

/* wait this long between setting a new PWM setting */
#define INTERVAL          CLOCK_SECOND/64  /* 更新step的頻率是1/64秒，每1/64秒step+1 */
/*---------------------------------------------------------------------------*/
PROCESS(pwmled_process, "PWM LED process");
AUTOSTART_PROCESSES(&pwmled_process);
/*---------------------------------------------------------------------------*/
/* PWM process; finds and sets the PWM. */
static struct etimer etr;
static uint8_t i = 1;     /* counter */
static uint8_t up = 1;    /* counting up or down? */

PROCESS_THREAD(pwmled_process, ev, data)
{
  PROCESS_POLLHANDLER(); /* 確認process有無在執行 */
  PROCESS_EXITHANDLER(simple_pwm_off();); /* 若結束則呼叫simple_pwm_off()，將pwm關掉 */
  PROCESS_BEGIN();

  simple_pwm_confpin(LEDRED_PIN); /* 定義pwm從哪個腳輸出，這裡用紅色LED */

  while(1) {

    simple_pwm_on(i); /* 輸出i值(若i=50, 則代表50% high 50% low) */

    /* find next PWM setting */
    if(up) {
      if(i < PWM_MAX - PWM_STEP) {
        i += PWM_STEP;
      } else {
        i = PWM_MAX;
        up = 0; //改為漸暗
      }
    } else {
      if(i > PWM_MIN + PWM_STEP) {
        i -= PWM_STEP;
      } else {
        i = PWM_MIN ;
        up = 1; //改為漸亮
      }
    }

    /* wait a little while */
    etimer_set(&etr, INTERVAL); /* 1/64秒 */
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etr));
  }
  PROCESS_END();
}
/*--------------------------------------------------------------------------.*/
