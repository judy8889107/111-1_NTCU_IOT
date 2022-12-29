/**
 * \addtogroup launchpad-examples
 *
 * @{
 */

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
 *    readadc-poll.c
 * \author
 *         Marcus Linderoth <linderoth.marcus@gmail.com>
 * \brief
 *    ADC reading example. One process periodically reads the ADC on one channel
 *    and updates a blink speed variable. Another sets a LED blink according to
 *    that variable.
 *
 */

#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/adc.h"

static uint16_t adcval;
static struct etimer et;
/*--------------------------------------------------------------------------*/
PROCESS(adc_reading_process, "ADC reading process");
AUTOSTART_PROCESSES(&adc_reading_process);
/* -------------------------------------------------------------------------- */
static void
adc_pollhandler(void)
{
  double c_temp, f_temp;
  c_temp = adcval * 1.5 / 1024;
  c_temp = (c_temp - 0.986) / 0.00355;
  f_temp = (c_temp * 9 / 5) + 32;
  // printf("ADC: %u\n", adcval);
  printf("%u\n", f_temp);
  // printf("aaa: %u\n", adcval);
  leds_toggle(LEDS_GREEN);
  if (adcval > 500)
  {
    leds_on(LEDS_RED);
  }
  else
  {
    leds_off(LEDS_RED);
  }
}
/*--------------------------------------------------------------------------*/
PROCESS_THREAD(adc_reading_process, ev, data)
{
  PROCESS_POLLHANDLER(adc_pollhandler());
  PROCESS_BEGIN();
  // printf("adc process start.\n");
  leds_off(LEDS_ALL);

  while (1)
  {
    /* read analog in A7, store the result in adcval and poll this process when done */
    adc_get_poll(TEMP, &adcval, PROCESS_CURRENT());
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
/** @} */
