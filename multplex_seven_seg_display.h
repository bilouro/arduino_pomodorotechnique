#include "Arduino.h"

const int digit[4] =  {11, 10, 9, 6}; //digit pin
const int seg[7] =  {A1, 3, 4, 5, A0, 7, 8}; //7 segments mapping
const int hour_separator_pin =  12;

int displaying, x;

#define SEGMENT_ON  LOW
#define SEGMENT_OFF HIGH

//#define DISPLAY_BRIGHTNESS 5 //(15.7mA current draw per digit)
//#define DISPLAY_BRIGHTNESS 2 //(11.4mA current draw per digit)
#define DISPLAY_BRIGHTNESS 1 //(5.9mA current draw per digit)

const int number_representation[10][7] =  {
         { //0
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_OFF,
         },
         { //1
                SEGMENT_OFF,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_OFF,
                SEGMENT_OFF,
                SEGMENT_OFF,
                SEGMENT_OFF,
         },
         { //2
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_OFF,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_OFF,
                SEGMENT_ON,
         },
         { //3
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_OFF,
                SEGMENT_OFF,
                SEGMENT_ON,
         },
         { //4
                SEGMENT_OFF,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_OFF,
                SEGMENT_OFF,
                SEGMENT_ON,
                SEGMENT_ON,
         },
         { //5
                SEGMENT_ON,
                SEGMENT_OFF,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_OFF,
                SEGMENT_ON,
                SEGMENT_ON,
         },
         { //6
                SEGMENT_ON,
                SEGMENT_OFF,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
         },
         { //7
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_OFF,
                SEGMENT_OFF,
                SEGMENT_OFF,
                SEGMENT_OFF,
         },
         { //8
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
         },
         { //9
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_ON,
                SEGMENT_OFF,
                SEGMENT_ON,
                SEGMENT_ON,
         }

     };

void display_setup() {
  for (x = 0; x < (sizeof(seg)/sizeof(int)); x++)   pinMode(seg[x], OUTPUT);
  for (x = 0; x < (sizeof(digit)/sizeof(int)); x++) pinMode(digit[x], OUTPUT);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);
  pinMode(hour_separator_pin, OUTPUT);
  digitalWrite(hour_separator_pin, HIGH);
}

void lightNumber(int numberToDisplay) {
    for (x = 0; x < (sizeof(seg)/sizeof(int)); x++)
        digitalWrite(seg[x], number_representation[numberToDisplay][x]);
}

void display_displayNumber(int toDisplay) {
    for (displaying = ((sizeof(digit)/sizeof(int))-1) ; displaying >= 0 ; displaying--) {
        //Show the first right number
        lightNumber(toDisplay % 10);
        //Shifting toDisplay value to right
        toDisplay /= 10;

        digitalWrite(digit[displaying], HIGH);
        delay(DISPLAY_BRIGHTNESS);
        digitalWrite(digit[displaying], LOW);
    }
}


