/*
  Code stolen from:
  Created 22 April 2007
  By David A. Mellis  <dam@mellis.org>
  modified 9 Apr 2012
  by Tom Igoe
  http://www.arduino.cc/en/Tutorial/Smoothing

  This example code is in the public domain.


*/


// Define the number of samples to keep track of.  The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input.  Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
const int numReadings = 5;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average
float p = 0;
long int t = 0;
long int t0 = 0;

//Change these paramters for your Arduino
int inputPin = A5;
int buttonPin = A8;
int laserPin = 13;
int buttonState = 0;
int on = 1;

void setup() {
  // initialize serial communication with computer:
  Serial.begin(2400);
  pinMode(buttonPin, INPUT);
  pinMode(inputPin, INPUT);
  pinMode(laserPin, OUTPUT);
  // initialize all the readings to 0:
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {
buttonState = digitalRead(buttonPin);

    if ((buttonState == 1) && (t-t0 > 1000)) {
      t0 = millis();
    }

    //Generates an exponential light curve that is supernova-ish
    t = millis();
    //p = 250*exp(-(t-t0)/24.);
    p = 250 - (t-t0);
    if (p >= 250){
      p = 250;}
    
    //Hack to fix timing. Edit to fir your circuit
    if ((t - t0 <= 7000) && (t >= 8000)) {
      Serial.print(1000);
      Serial.print(",");
      Serial.print(700);
      Serial.print(",");
      Serial.println(average);
    } 
  
    if (p <= 0){
      p = 0;}

   analogWrite(laserPin, p);
  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(inputPin);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
  // send it to the computer as ASCII digits
  

  delay(1);        // delay in between reads for stability


    





  
}
