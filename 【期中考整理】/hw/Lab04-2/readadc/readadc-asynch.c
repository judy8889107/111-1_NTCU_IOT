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
 *    readadc-asynch.c
 * \author
 *         Marcus Linderoth <linderoth.marcus@gmail.com>
 * \brief
 *    ADC reading example. One process periodically reads the ADC on one channel
 *    and updates a blink speed variable, plus blinks an LED. Another sets a LED
 *    blink according to that variable. Program execution is not blocked while
 *    ADC is converting.
 */

#include <stdio.h>
#include <stdbool.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/adc.h"
#include "dev/button.h"
#include "dev/serial-line.h"

static volatile uint16_t adc = 0;
bool adc_off = true;
/*--------------------------------------------------------------------------*/
PROCESS(adc_reading_process, "ADC reading process");
PROCESS(button_click, "Button Process");
PROCESS(red_process, "Toggle red LED");
PROCESS(green_process, "Toggle green LED");
PROCESS(input_process, "Serial Reader");
AUTOSTART_PROCESSES(&input_process, &button_click);
/*--------------------------------------------------------------------------*/
static struct etimer et;
PROCESS_THREAD(adc_reading_process, ev, data)
{
  PROCESS_POLLHANDLER();
  PROCESS_EXITHANDLER();
  PROCESS_BEGIN();

  while (1)
  {
    adc_get_noblock(TEMP, &adc);
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

PROCESS_THREAD(button_click, ev, data)
{
  PROCESS_POLLHANDLER();
  PROCESS_EXITHANDLER();
  PROCESS_BEGIN();
  while (1)
  {
    PROCESS_WAIT_EVENT_UNTIL(ev == button_event);

    double ans;
    ans = adc * 1.5 / 1024;
    ans = (ans - 0.986) / 0.00355;
    if (adc_off)
    {
      printf("You turn off ADC.\n");
      printf("Please turn on ADC.\n");
    }
    else
    {
      printf("adc: %u\n", adc);
      printf("temp: %u\n", (uint16_t)ans);
    }
  }
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

static struct etimer et_red;

PROCESS_THREAD(red_process, ev, data)
{
  PROCESS_POLLHANDLER();
  PROCESS_EXITHANDLER();
  PROCESS_BEGIN();

  while (1)
  {
    leds_toggle(LEDS_RED);
    etimer_set(&et_red, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et_red));
  }
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/

static struct etimer et_green;

PROCESS_THREAD(green_process, ev, data)
{
  PROCESS_POLLHANDLER();
  PROCESS_EXITHANDLER();
  PROCESS_BEGIN();

  while (1)
  {
    leds_toggle(LEDS_GREEN);
    etimer_set(&et_green, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et_green));
  }
  PROCESS_END();
}

/*---------------------------------------------------------------------------*/

PROCESS_THREAD(input_process, ev, data)
{
  PROCESS_BEGIN();
  // printf("serial:starting serial echo\n");
  while (1)
  {
    char *buf;
    PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message);
    buf = data;

    if (!strncmp(buf, "red", 3))
    {
      process_start(&red_process, NULL);
    }
    if (!strncmp(buf, "green", 5))
    {
      process_start(&green_process, NULL);
    }
    if (!strncmp(buf, "off", 3))
    {
      process_exit(&red_process);
      process_exit(&green_process);
      leds_off(LEDS_GREEN);
      leds_off(LEDS_RED);
    }
    if (!strncmp(buf, "all", 3))
    {
      process_exit(&red_process);
      process_exit(&green_process);
      leds_on(LEDS_GREEN);
      leds_on(LEDS_RED);
    }

    if (!strncmp(buf, "adc_on", 6))
    {
      process_start(&adc_reading_process, NULL);
      printf("You turn on ADC.\n");
      adc_off = false;
    }
    if (!strncmp(buf, "adc_off", 7))
    {
      process_exit(&adc_reading_process);
      printf("You turn off ADC.\n");
      adc_off = true;
    }
  }
  PROCESS_END();
}

/** @} */
