#include <Arduino.h>

#include "pwm.h"

struct PWM pwm1 = {
    .pin = 5,
    .r1 = 0.5, .r2 = 2.5,
    .offset = 0,
    .ledc_channel = 0,
    .ledc_freq = 156250,
    .ledc_resolution = 9
};

struct PWM pwm2 = {
    .pin = 19,
    .r1 = 1.5, .r2 = 4.5,
    .offset = 0.37, 
    .ledc_channel = 2,
    .ledc_freq = 156250,
    .ledc_resolution = 9
};

// Example command: "56.7%"
// commands are assumed to be denoted perline and end with a percent
// that sets the throttl to 56.7%
void parse_cmd(char *cmd_buff, size_t len) { 
  float percent = atof(cmd_buff);

  Serial.print("PWM1: ");
  pwm_percent(pwm1, 3.3, percent);

  Serial.print("PWM2: ");
  pwm_percent(pwm2, 5.0, percent);

  Serial.println("");
}

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(1500); // for slow typers

  // set up PWM pins
  ledcSetup(pwm1.ledc_channel, pwm1.ledc_freq, pwm1.ledc_resolution);
  ledcAttachPin(pwm1.pin, pwm1.ledc_channel);

  ledcSetup(pwm2.ledc_channel, pwm2.ledc_freq, pwm2.ledc_resolution);
  ledcAttachPin(pwm2.pin, pwm2.ledc_channel);
}

void loop() {
  char cmd_buff[32] = {0};
  if (Serial.available() > 0) { 
    Serial.readBytes(cmd_buff, sizeof(cmd_buff)-1); // buffer overflow a nono

    Serial.print("Command: ");
    Serial.println(cmd_buff);
    parse_cmd(cmd_buff, sizeof(cmd_buff));

    // zero out buffer 
    for (int i = 0; i < sizeof(cmd_buff); i++) 
      cmd_buff[i] = '\0';
  }

  delay(100);
}
