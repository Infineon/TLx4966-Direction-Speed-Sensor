#include <Arduino.h>

#ifdef ARDUINO_AVR_UNO
#define LED1 10
#define LED2 11
#endif

const int spdPin = 4;     // pin connected to Q2
const int dirPin = 9;     // pin connected to Q1
const int polePairs = 1;  // multiple pole pairs of magnets supported. 1 = one north and one south pole

int ms;  // holds number of milliseconds between two pulses of the speed pin

unsigned long timer;    // used for time-measurement between pulses
bool enableSpd = true;  // used for edge-detection of speed signal (as Interrupt is not possible with Q1 of S2Go)

void setup() {

  Serial.begin(1000000);
  pinMode(spdPin, INPUT_PULLUP);
  pinMode(dirPin, INPUT_PULLUP);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  timer = millis();

}

void loop() {
  // put your main code here, to run repeatedly:
  if(enableSpd && digitalRead(spdPin))  //rising edge of signal
  {
    spdPulse();         // time is set
    printOut();         // print data to serial monitor
    enableSpd=false;
  }
  if(!enableSpd && !digitalRead(spdPin))  // falling edge of signal
  {
    enableSpd=true;     // next rising edge will call spdPulse() again
  }

  // set LEDs according to pin-state of Q1(direction) and Q2(speed)
  digitalWrite(LED1, digitalRead(spdPin));
  digitalWrite(LED2, digitalRead(dirPin));
}

// refresh time since last call of spdPulse
void spdPulse()
{
  ms = millis()-timer;
  timer = millis();
}

// print data to serial monitor
void printOut()
{
  double rpm = 60000.0/(ms*polePairs); // calculation of RPM out of ms and polePairs
  Serial.print("Dir: ");
  Serial.print(digitalRead(dirPin));
  Serial.print("\tTime: ");
  Serial.print(ms);
  Serial.print(" ms\tSpeed: ");
  Serial.print(rpm);
  Serial.println(" RPM");
}
