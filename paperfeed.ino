#include <avr/interrupt.h>    //Include the Interrupt Library

int T2Speed = 238;            // Setting the Interrupt Speed?
int kuris = 0;                //Direction flag
const int channelA = 2;      // Setting the pin number for channel A as 2
const int channelB = 3;      // Setting the pin number for channel A as 3

volatile byte tmrCount = 0; //Timer2 overflow interrupt vector handler
ISR(TIMER2_OVF_vect) {
  TCNT2 = T2Speed;          //reset timer
  if (kuris == 0) {
    switch (tmrCount) {
      case 0:
        digitalWrite(channelA, HIGH);
        break;
      case 1:
        digitalWrite(channelB, HIGH);
        break;
      case 2:
        digitalWrite(channelA, LOW);
        break;
      case 3:
        digitalWrite(channelB, LOW);
    }
  }
  else {
    switch (tmrCount) {
      case 0:
        digitalWrite(channelB, HIGH);
        break;
      case 1:
        digitalWrite(channelA, HIGH);
        break;
      case 2:
        digitalWrite(channelB, LOW);
        break;
      case 3:
        digitalWrite(channelA, LOW);
    }


  }
  tmrCount++;
  if (tmrCount >= 4) {
    tmrCount = 0;
  }
  TIFR2 = 0x00;
};

void setup() {
  //Serial.begin(115200);
  pinMode(channelA, OUTPUT);
  pinMode(channelB, OUTPUT);
  TCCR2A = 0;           //Timer2 Settings: WGM mode 0
  TCCR2B = _BV(CS22);   //Timer2 Settings: Timer Prescaler /64
  TIMSK2 = _BV(TOIE2);  //Timer2 Overflow Interrupt Enable
  TCNT2 = T2Speed;      //reset timer
}

void loop() {
  int sensorValue = analogRead(A0);
  //if (sensorValue < 1) {
  //  TIMSK2 &= ~(1 << TOIE2); //ISJUNGTI TIMERI
  //}
  //else  {
  //  TIMSK2 = _BV(TOIE2); //IJUNGTI TIMERI
  //}
  if (sensorValue < 250 && sensorValue > 100) {
    //Serial.println("PIRMYN");
    kuris = 1;
    T2Speed = 248;
  }
  else {
    if (sensorValue > 315) {
      //Serial.println("ATGAL"); // toks pats kaip letai
      kuris = 0;
      T2Speed = 248;
    }
    else {
      if (sensorValue < 315 && sensorValue > 250) {
        //Serial.println("LETAI"); //toks pats kaip atgal
        kuris = 0;
        T2Speed = 8;
      }
    }
  }
}
