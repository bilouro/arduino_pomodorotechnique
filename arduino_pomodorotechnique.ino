/*
Copyright (c) 2012, Victor Hugo Bilouro
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

Redistributions of source code must retain the above copyright notice,
this list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in the
documentation and/or other materials provided with the distribution.

Neither the name of Victor Hugo Bilouro nor the names of its 
contributors may be used to endorse or promote products derived from 
this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Author: Victor Hugo Bilouro 

Description: This code enables to do a Pomodoro.
(http://www.pomodorotechnique.com/)

External Requirements:
- 01 arduino;
- 03 leds connected on pwd ports;
- 01 buzz;

External Source Code:
- (http://arduino.cc/en/Tutorial/Tone)

*/

#include "multplex_seven_seg_display.h"

const unsigned long minute = 60000;
const int fast_beep_mills = 10;
const int minute_alert_beep_mills = 1;
const int buzz = 2;
const int buzz_volume = 100; //0-255 via analogWrite

unsigned long start_time, actual_time;
unsigned long time_elapsed, time_elapsed_in_minute, seconds;
unsigned long beep_timer = 0;
unsigned long restart_time;

//int led_pin[] = { 11, 10, 9 };
int led_pin[] = {  };
//int turnon_led_minute[] = { 0, 20, 25 };
int turnon_led_minute[] = {  };
int restart_on_minute = 30;

int beep_minute[]       =  { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 };
int beep_minute_status[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
int i;

void setup(){
  display_setup();

  pinMode(buzz, OUTPUT); 
  for (i = 0; i < (sizeof(led_pin)/sizeof(int)); i++) {
    pinMode(led_pin[i], OUTPUT); 
  }
  Serial.begin(9600);
  mytone(fast_beep_mills);
  setup_start();
}

void setup_start(){
  start_time = millis();
  restart_time = start_time + ( restart_on_minute * minute );
  for (i = 0; i < (sizeof(beep_minute_status)/sizeof(int)); i++) 
    beep_minute_status[i] = 0;
}

void loop() {
  update_timers();
  check_leds();
  check_beeps();
  debug();
  check_restart();
  display_displayNumber( (time_elapsed_in_minute*100) + seconds);
}

void update_timers() {
  actual_time = millis();
  time_elapsed = actual_time - start_time;
  time_elapsed_in_minute = time_elapsed / minute;
  seconds = ( time_elapsed - (time_elapsed_in_minute * minute) ) /1000;
}

void check_leds() {
  for (i = 0; i < (sizeof(turnon_led_minute)/sizeof(int)); i++) {
     if (time_elapsed_in_minute >= turnon_led_minute[i])
       digitalWrite(led_pin[i], HIGH);
    else
       digitalWrite(led_pin[i], LOW);
  }
}

void check_beeps() {
  for (i = 0; i < (sizeof(beep_minute)/sizeof(int)); i++) {
     if (time_elapsed_in_minute >= beep_minute[i] && beep_minute_status[i] == 0) {
       digitalWrite(buzz, HIGH);
       beep_minute_status[i] = 1;
       beep_timer = actual_time + minute_alert_beep_mills;
     }
  }
  if (beep_timer <= actual_time) {
       digitalWrite(buzz, LOW);
  }
}

void check_restart() {
  if (actual_time >= restart_time) {
    mytone(fast_beep_mills*2);
    setup_start();
  }
}

void mytone(int duration) {
  for (i = 0; i < 3; i++) {
    digitalWrite(buzz, HIGH);
    delay(duration*2);
    digitalWrite(buzz, LOW);
    delay(duration*2);
  }
}


void debug() {
  Serial.print(actual_time);
  Serial.print(" / ");
  Serial.print(time_elapsed);
  Serial.print(" / ");
  Serial.println(time_elapsed_in_minute);
}




