#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/adc.h"

static volatile uint16_t adc = 0; 
PROCESS(adc_reading_process, "ADC reading process");
PROCESS(blink_process, "My Process");
AUTOSTART_PROCESSES(&adc_reading_process, &blink_process);
static uint16_t time = 0;
static struct etimer etb;

PROCESS_THREAD(blink_process, ev, data) {
  PROCESS_POLLHANDLER();
  PROCESS_EXITHANDLER();
  PROCESS_BEGIN();
  while(1) {
    leds_toggle(LEDS_RED);
    if(adc > 0) { 
      time = (CLOCK_SECOND*adc)/1024; 
    } else { 
      time = CLOCK_SECOND/64;
    }
    if(time == 0) {
      time = 1;
    }
    etimer_set(&etb, time);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etb));
  }
  PROCESS_END();}

static struct etimer et;
PROCESS_THREAD(adc_reading_process, ev, data)
{
  PROCESS_BEGIN();
  while(1) {
    adc_get_noblock(TEMP, &adc);
    leds_toggle(LEDS_GREEN);
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));

    printf("TADC: %u\n", adc);
  }
  PROCESS_END();
}

