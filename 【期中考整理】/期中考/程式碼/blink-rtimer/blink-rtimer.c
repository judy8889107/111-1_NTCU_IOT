#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"


PROCESS(blink_process, "Blink");
AUTOSTART_PROCESSES(&blink_process);

static void rtcb(struct rtimer *r, void *d)
{
  leds_off(LEDS_RED);
}
static struct rtimer rt;
static struct etimer et;

PROCESS_THREAD(blink_process, ev, data)
{
  PROCESS_BEGIN();
  while(1) {
    leds_on(LEDS_RED);
    
    rtimer_set(&rt, RTIMER_NOW() + RTIMER_SECOND/4, 1, rtcb, NULL); 
    leds_toggle(LEDS_GREEN);
    etimer_set(&et, CLOCK_SECOND/2); 
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
    
  }
  PROCESS_END();
}

