#ifndef gpio_sensors_h
#define gpio_sensors_h

#include "gpio_jsn_sr04t.h"

void gpio_sensors_init() {
  gpio_jsn_sr04t_try_init();
}

#endif