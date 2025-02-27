#ifndef w1_sensors_h
#define w1_sensors_h

#include <OneWire.h>

#define ONE_WIRE_BUS G8          // Pin to which 1-wire bus is connected to esp32
OneWire oneWire(ONE_WIRE_BUS);

#include "1w_ds18b20.h"

void one_wire_sensors_scan() {
  pinMode(ONE_WIRE_BUS, INPUT);
  one_wire_dallas_try_init();
}

#endif

