#include <Arduino.h>

// Struct for aggregating information about the particular PWM pin
struct PWM {
  int pin;
  float r1, r2;   // voltage ranges for PWM signal (eg. 0.5v to 2.5v)
  float offset;   // offset 

  // ledc_ are all for the Arduino PWM interface 
  int ledc_channel; 
  int ledc_freq;
  int ledc_resolution;
};

struct PWM pwm1 = {
    .pin = 5,
    .r1 = 0.5, .r2 = 2.5,
    .offset = 0,
    .ledc_channel = 0,
    .ledc_freq = 10000,
    .ledc_resolution = 8
};

struct PWM pwm2 = {
    .pin = 19,
    .r1 = 1.5, .r2 = 4.5,
    .offset = 0.37, 
    .ledc_channel = 2,
    .ledc_freq = 10000,
    .ledc_resolution = 8
};

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

void setup() {
  Serial.begin(9600);

  ledcSetup(pwm1.ledc_channel, pwm1.ledc_freq, pwm1.ledc_resolution);
  ledcAttachPin(pwm1.pin, pwm1.ledc_channel);

  ledcSetup(pwm2.ledc_channel, pwm2.ledc_freq, pwm2.ledc_resolution);
  ledcAttachPin(pwm2.pin, pwm2.ledc_channel);
}

void loop() {
  Serial.print("PWM1: ");
  pwm_percent(pwm1, 3.3, 50.0);

  Serial.print("PWM2: ");
  pwm_percent(pwm2, 5.0, 0.0);

  delay(1000);
}