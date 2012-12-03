#include "Arduino.h"

const int digit[4] =  {12, 8, 7, 6}; //digit pin
const int seg[4] =  {4, 3, 2, 5}; //cbda -- [[ cbad]]
int displaying, x;

//int display_brightness = 5000; //(15.7mA current draw per digit)
//int display_brightness = 2000; //(11.4mA current draw per digit)
int display_brightness = 1000; //(5.9mA current draw per digit)
  
const int number_representation[10][4] =  {
         { //0
                LOW,
                LOW,
                LOW,
                LOW
         },
         { //1
                LOW,
                LOW,
                LOW,
                HIGH
         },
         { //2
                LOW,
                LOW,
                HIGH,
                LOW
         },
         { //3
                LOW,
                LOW,
                HIGH,
                HIGH
         },
         { //4
                LOW,
                HIGH,
                LOW,
                LOW
         },
         { //5
                LOW,
                HIGH,
                LOW,
                HIGH,
         },
         { //6
                LOW,
                HIGH,
                HIGH,
                LOW,
         },
         { //7
                LOW,
                HIGH,
                HIGH,
                HIGH,
         },
         { //8
                HIGH,
                LOW,
                LOW,
                LOW,
         },
         { //9
                HIGH,
                LOW,
                LOW,
                HIGH,
         }

     };

void display_setup() {
  for (x = 0; x < (sizeof(seg)/sizeof(int)); x++)   pinMode(seg[x], OUTPUT);
  for (x = 0; x < (sizeof(digit)/sizeof(int)); x++) pinMode(digit[x], OUTPUT);
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
        delayMicroseconds(display_brightness);
        digitalWrite(digit[displaying], LOW);
    }
}


