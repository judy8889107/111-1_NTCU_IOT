#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
PROCESS(red_process, "Red light process");
PROCESS(blink_process, "Blink");
AUTOSTART_PROCESSES(&blink_process);
static struct etimer etr; 
PROCESS_THREAD(red_process, ev, data)
{
  PROCESS_POLLHANDLER(); 
  PROCESS_EXITHANDLER(); 
  PROCESS_BEGIN();       
  while(1) {
    leds_toggle(LEDS_RED);
    etimer_set(&etr, CLOCK_SECOND/8); 
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etr)); 
  }
  PROCESS_END();
}
PROCESS_THREAD(blink_process, ev, data)
{
  PROCESS_BEGIN();

  etimer_set(&et, CLOCK_SECOND);
  PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  process_start(&red_process, NULL);

  while(1) {
    
    if(clock_seconds() & 1) { 
      leds_on(LEDS_GREEN);
    } else {
      leds_off(LEDS_GREEN);
    }
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }
  PROCESS_END();
} 
