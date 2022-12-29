#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/button.h"

PROCESS(button_process, "Button process");
PROCESS(blink_process, "Blink process");
AUTOSTART_PROCESSES(&blink_process, &button_process);

PROCESS_THREAD(button_process, ev, data)
{
  PROCESS_POLLHANDLER();
  PROCESS_EXITHANDLER();
  PROCESS_BEGIN();
  static uint8_t btnpresses = 0;
  while(1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == button_event);
    leds_toggle(LEDS_RED);
    btnpresses++;
    if(btnpresses == 5) {
      process_exit(&blink_process);
    }
  }
  PROCESS_END();
}
static struct etimer et;
PROCESS_THREAD(blink_process, ev, data)
{
  PROCESS_BEGIN();
  while(1) {
    leds_toggle(LEDS_GREEN);
    etimer_set(&et, CLOCK_SECOND/8);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }
  PROCESS_END();
}
