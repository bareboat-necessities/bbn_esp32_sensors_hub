#ifndef w1_ds18b20_h
#define w1_ds18b20_h

#include <OneWire.h>
#include <DallasTemperature.h>

#include "NmeaXDR.h"
#include "Nmea0183Msg.h"

DallasTemperature sensors(&oneWire);

void byte_to_hex_ascii(unsigned char byte, char hex_str[3]) {
  sprintf(hex_str, "%02X", byte);
}

void addressToStr(DeviceAddress deviceAddress, char str[17]) {
  for (uint8_t i = 0; i < 8; i++) {
    char a[3];
    byte_to_hex_ascii(deviceAddress[i], a);
    str[2 * i] = a[0];
    str[2 * i + 1] = a[1];
  }
  str[16] = (char)0;
}

void one_wire_dallas_sensors_report() {
  int count = sensors.getDeviceCount();
  if (count > 0) {
    sensors.requestTemperatures();
    for (int i = 0; i < count; i++) {
      DeviceAddress deviceAddress;
      if (sensors.getAddress(deviceAddress, i)) {
        char str[17];
        addressToStr(deviceAddress, str);
        gen_nmea0183_msg("$BBXDR,C,%s", (String(sensors.getTempCByIndex(i)) + ",C,TEMP_" + String(str)).c_str());  // C
      }
    }
  }
}

void one_wire_dallas_try_init() {
  sensors.begin();
  int count = sensors.getDeviceCount();
  if (count > 0) {
    gen_nmea0183_msg("$BBTXT,01,01,01,TEMPERATURE 1wire sensors found=%s", String(count).c_str());
    for (int i = 0; i < count; i++) {
      DeviceAddress deviceAddress;
      if (!sensors.getAddress(deviceAddress, i)) {
        gen_nmea0183_msg("$BBTXT,01,01,02,TEMPERATURE unable to find 1wire sensor address for device=%s", String(i).c_str());
      } else {
        char str[17];
        addressToStr(deviceAddress, str);
        gen_nmea0183_msg("$BBTXT,01,01,01,TEMPERATURE found 1wire sensor address=%s", str);
      }
    }
    app.onRepeat(5000, []() {
      one_wire_dallas_sensors_report();
    });
  }
}

#endif
