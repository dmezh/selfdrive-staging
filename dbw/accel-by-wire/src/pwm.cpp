#include <Arduino.h>
#include "pwm.h"

// percet is assumed to be 0 - 100
void pwm_percent(struct PWM pwm, float vdd, float percent) {
  float voltage = (percent/100.0) * (pwm.r2 - pwm.r1) + (pwm.r1 + pwm.offset); 
  int pwm_level = voltage * (256.0/vdd);

  Serial.print("Accel throttle: "); 
  Serial.print(percent);
  Serial.println("%");

  Serial.print("\tDesired voltage: "); 
  Serial.print(voltage - pwm.offset);
  Serial.print("V || Output voltage: "); 
  Serial.print(voltage);
  Serial.println("V");

  ledcWrite(pwm.ledc_channel, pwm_level);
}
