
#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/adc.h"

PROCESS(adc_reading_process, "ADC reading process");
PROCESS(heartbeat_process, "My Process");
AUTOSTART_PROCESSES(&heartbeat_process, &adc_reading_process);
static struct etimer etb;

PROCESS_THREAD(heartbeat_process, ev, data) {
  PROCESS_POLLHANDLER();
  PROCESS_EXITHANDLER();
  PROCESS_BEGIN();
  leds_off(LEDS_ALL);

  while(1) {
    
    leds_off(LEDS_GREEN);
    etimer_set(&etb, CLOCK_SECOND/32); 
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etb));
    leds_on(LEDS_GREEN);
    etimer_set(&etb, CLOCK_SECOND - CLOCK_SECOND/32);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etb));
  }
  PROCESS_END();
}
PROCESS_THREAD(adc_reading_process, ev, data)
{
  PROCESS_BEGIN();
  while(1) {
    adc_get_event(A7, PROCESS_CURRENT());
    PROCESS_WAIT_EVENT_UNTIL(ev == adc_event);
    if(data != NULL) {
      printf("ADC: %u\n", *((uint16_t*)data));
      if(*((uint16_t*)data) > 500) {
        leds_on(LEDS_RED);
      } else {
        leds_off(LEDS_RED);
      }
    }
  }
  PROCESS_END();
}

