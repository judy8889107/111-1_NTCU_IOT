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
 *         Blink example application,
 *          Demonstrates starting another process from one, setting timers, using LEDs.
 *          expected result from running it:
 *          red LED flashing fast (toggling every 125 ms), green every second.
 * \author
 *         Marcus Linderoth <linderoth.marcus@gmail.com>
 */

#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"

/* -------------------------------------------------------------------------- */
PROCESS(red_process, "Red light process");
PROCESS(blink_process, "Blink");
AUTOSTART_PROCESSES(&blink_process);
/*---------------------------------------------------------------------------*/
/* This process is started from the other process and blinks an LED fast.
*/
static struct etimer etr; //宣告自己的timer叫etr，跟下面的timer名字要不同
PROCESS_THREAD(red_process, ev, data)
{
  PROCESS_POLLHANDLER(); //process叫起來
  PROCESS_EXITHANDLER(); //process要是exit狀態
  PROCESS_BEGIN();       //process啟動
  while(1) {
    leds_toggle(LEDS_RED);
    etimer_set(&etr, CLOCK_SECOND/8); // 1/8秒，CLOCK_SECOND = 1秒
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etr)); //等訊號出現(超過1/8秒)>while迴圈
  }
  PROCESS_END();
}

/*--------------------------------------------------------------------------*/
/* this process starts another process and then periodically blinks an LED */
static struct etimer et; //宣告自己的timer叫et

PROCESS_THREAD(blink_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&et, CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  process_start(&red_process, NULL);

  while(1) {
    /* unnecessary, messy but wanted sth else than what the red process
     * is doing so instead of just toggling, turn on when even seconds and
     * turn off on uneven seconds.
     */
    if(clock_seconds() & 1) { //判斷奇數偶數second來控制綠燈開關(偶數秒打開，奇數秒關閉)
      leds_on(LEDS_GREEN);
    } else {
      leds_off(LEDS_GREEN);
    }
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

