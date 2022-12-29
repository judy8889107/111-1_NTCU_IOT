#include <stdio.h>
#include <string.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/serial-line.h"

PROCESS(serial_read_process, "Serial Reader");
AUTOSTART_PROCESSES(&serial_read_process);

PROCESS_THREAD(serial_read_process, ev, data) {
  PROCESS_BEGIN();
  printf("starting serial echo\n");
  while (1) {
    char* buf;
    PROCESS_WAIT_EVENT_UNTIL(ev == serial_line_event_message);
    buf = data;

    // if(!strncmp(buf, "red", 3)) {
    //   leds_toggle(LEDS_RED);
    // } else if (!strncmp(buf, "green", 5)) {
    //   leds_toggle(LEDS_GREEN);
    // } else if (!strncmp(buf, "off", 3)) {
    //   leds_off(LEDS_GREEN | LEDS_RED);
    // }

    printf("Got:%s\n", buf);
  }
  PROCESS_END();
}

static struct etimer et;

PROCESS_THREAD(blink_process, ev, data)
{
  PROCESS_BEGIN();
  while(1) {
    printf("Time since bootup: %lu\n", clock_seconds());
    etimer_set(&et, CLOCK_SECOND);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&et));
  }
  PROCESS_END();
}

