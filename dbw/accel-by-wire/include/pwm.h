#pragma once

// Struct for aggregating information about the particular PWM pin
struct PWM {
  int pin;
  float r1, r2;   // voltage ranges for PWM signal (eg. 0.5v to 2.5v)
  float offset;   // offset that accounts for inefficeincies and voltage drops 

  // values for esp32 ledc commands 
  int ledc_channel; 
  int ledc_freq;    // max freq = 80000000/2^resolution
  int ledc_resolution;
};

// percet is assumed to be 0 - 100
void pwm_percent(struct PWM pwm, float vdd, float percent);
