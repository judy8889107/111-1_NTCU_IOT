
#include <stdio.h>
#include "contiki.h"
#include "dev/leds.h"
#include "dev/button.h"
#include "simple-pwm.h"
#define LEDRED_PIN       (0)   // P1.0
#define LEDGRN_PIN       (6)   // P1.6
#define LEDBIG_PIN       (7)   // P1.7
#define PWM_MIN           0
#define PWM_MAX           100
#define PWM_STEP          1
#define INTERVAL          CLOCK_SECOND/64

PROCESS(pwmled_process, "PWM LED process");
AUTOSTART_PROCESSES(&pwmled_process);
static struct etimer etr;
static uint8_t i = 1;    
static uint8_t up = 1;    
PROCESS_THREAD(pwmled_process, ev, data)
{ PROCESS_POLLHANDLER();
  PROCESS_EXITHANDLER(simple_pwm_off(););
  PROCESS_BEGIN();
  simple_pwm_confpin(LEDRED_PIN);
  while(1) {
    simple_pwm_on(i);
    if(up) {
      if(i < PWM_MAX - PWM_STEP) {
        i += PWM_STEP;
      } else {
        i = PWM_MAX;
        up = 0;
      }
    } else {
      if(i > PWM_MIN + PWM_STEP) {
        i -= PWM_STEP;
      } else {
        i = PWM_MIN ;
        up = 1;
      }
    }
    etimer_set(&etr, INTERVAL);
    PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&etr));
  }
  PROCESS_END();
}
