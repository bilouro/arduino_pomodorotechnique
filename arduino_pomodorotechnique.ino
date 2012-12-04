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
- 01 rgb-led; (+3 (220Ohms) resistors)
- 01 buzz;
- 02 push-button;
- 01 potentiometer
- 02 female-socket (8 pins, 10 pins)
- 01 4 digit 7 segments display
- 01 SN7447n (IC BCD-to-7segments)
- 01 board
- 17 wires

External Source Code:
- (http://arduino.cc/en/Tutorial/Tone)

*/

#include "multplex_seven_seg_display.h"

const unsigned long minute = 60000;
const int fast_beep_mills = 5;
const int minute_alert_beep_mills = 1;
const int buzz = A0;
const int pb1 = A2;
const int pb2 = A3;
const int potentiometer = A1;
unsigned long start_time, actual_time;
unsigned long time_elapsed, time_elapsed_in_minute, seconds;
unsigned long last_second = 99;
unsigned long beep_timer = 0;
unsigned long restart_time;
int zigzag_num = random(1 , 254);
int zigzag_way = random(0 , 1);

int rgb_led_pin_list[] = { 11, 10, 9 };
int color_change_list[] = { 0, 20, 25 }; //1 change per minute only.
const int color_set[3][3] =  { //must have item here per each item in color_change_list
         {
                255, //r
                1,  //g
                0,  //b
         },
         {
                255, //r
                255,  //g
                0,  //b
         },
         {
                1,  //r
                255,  //g
                0,  //b
         }};


int restart_on_minute = 30;

int beep_list[]       =  { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25 };
int beep_status_list[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 }; //must follows beep_list
int i, change, color;

void setup(){
  display_setup();

  //Serial.begin(9600);

  pinMode(buzz, OUTPUT);
  pinMode(pb1, INPUT_PULLUP); 
  pinMode(pb2, INPUT_PULLUP); 
  
  for (i = 0; i < (sizeof(rgb_led_pin_list)/sizeof(int)); i++) {
    pinMode(rgb_led_pin_list[i], OUTPUT);
  }

  play_beep(fast_beep_mills);
  setup_start();
}

void setup_start(){
  start_time = millis();
  restart_time = start_time + ( restart_on_minute * minute );
  for (i = 0; i < (sizeof(beep_status_list)/sizeof(int)); i++)
    beep_status_list[i] = 0;
}

void loop() {
  check_display_brightness();
  check_buttons();
  update_timers();
  check_leds();
  check_beeps();
  //debug();
  check_restart();
  display_displayNumber( (time_elapsed_in_minute*100) + seconds);
}

void update_timers() {
  actual_time = millis();
  time_elapsed = actual_time - start_time;
  time_elapsed_in_minute = time_elapsed / minute;
  seconds = ( time_elapsed - (time_elapsed_in_minute * minute) ) /1000;
  if (seconds != last_second) {
    last_second = seconds;
    on_change_second();
  }
  zigzag();  
}

void on_change_second() {
}

void zigzag() {
  if (zigzag_way) {
    zigzag_num += 1;
  } else {
    zigzag_num -= 1;
  }
  if (zigzag_num <= 0 || zigzag_num >=250) {
    zigzag_way = zigzag_way == 1 ? 0 : 1;
  }
}


void check_display_brightness() {
  display_brightness = constrain(
                         map(analogRead(potentiometer), 0, 1024, 30, 3000),
                         30, 3000);
}

void check_buttons() {
  int button1 = digitalRead(pb1);
  if (button1 == LOW) {
    //wait for button release 
    while (digitalRead(pb1) == LOW) delay(500);
    restart();
  }
}

int random_if_zero( int color_set ) {
  return color_set == 0 ? random(0, 20): color_set;
}
int zigzag_num_if_zero( int color_set ) {
  return color_set == 0 ? constrain(map(zigzag_num, 0, 255, 0, 200), 0, 200): color_set;
}


void check_leds() {
  for (change = 0; change < (sizeof(color_change_list)/sizeof(int)); change++) {
    if (time_elapsed_in_minute >= color_change_list[change])
       for (color = 0; color < (sizeof(rgb_led_pin_list)/sizeof(int)); color++)
         analogWrite(rgb_led_pin_list[color], zigzag_num_if_zero(color_set[change][color]) );
  }
}

void check_beeps() {
  for (i = 0; i < (sizeof(beep_list)/sizeof(int)); i++) {
     if (time_elapsed_in_minute >= beep_list[i] && beep_status_list[i] == 0) {
       digitalWrite(buzz, HIGH);
       beep_status_list[i] = 1;
       beep_timer = actual_time + minute_alert_beep_mills;
     }
  }
  if (beep_timer <= actual_time) {
       digitalWrite(buzz, LOW);
  }
}

void restart() {
    play_beep(fast_beep_mills*2);
    setup_start();
}

void check_restart() {
  if (actual_time >= restart_time) {
     restart();
  }
}

void play_beep(int duration) {
  for (i = 0; i < 3; i++) {
    digitalWrite(buzz, HIGH);
    delay(duration);
    digitalWrite(buzz, LOW);
    delay(duration);
  }
}

void debug() {
  Serial.print(actual_time);
  Serial.print(" / ");
  Serial.print(time_elapsed);
  Serial.print(" / ");
  Serial.println(time_elapsed_in_minute);
}




