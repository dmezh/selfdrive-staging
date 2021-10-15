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

// Example command: "p1:56.7%"
// that sets pwm1 to 56.7%
void parse_cmd(char *cmd_buff, size_t len) { 
  char pin[4] = {0};
  char percent[8] = {0};
  size_t i, j;  // i: index local buffer, j: index cmd_buff

  // get pin from cmd_buff
  for (i = 0, j = 0; i < sizeof(pin)-1 && j < len; i++, j++) { 
    if (cmd_buff[j] == ':'){ 
      j++;
      break;
    }
    pin[i] = cmd_buff[j];
  }

  // get throttle percent from cmd_buff
  for (i = 0; i < sizeof(pin)-1 && j < len; i++, j++) { 
    if (cmd_buff[j] == '%'){ 
      break;
    }
    percent[i] = cmd_buff[j];
  }

  float v_percent = atof(percent);

  if (pin[0] == 'p') {
    if (pin[1] == '1') { 
      Serial.print("PWM1: ");
      pwm_percent(pwm1, 3.3, v_percent);
    }
    if (pin[1] == '2') {
      Serial.print("PWM2: ");
      pwm_percent(pwm2, 5.0, v_percent);
    }
  }
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
