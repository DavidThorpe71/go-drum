#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Encoder.h>
#include <Chrono.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioSynthKarplusStrong  string1;        //xy=110,421
AudioSynthSimpleDrum     drum1;          //xy=111,234
AudioSynthSimpleDrum     drum2;          //xy=112,276
AudioSynthSimpleDrum     drum3;          //xy=113,319
AudioSynthKarplusStrong  string3;        //xy=115,506
AudioSynthKarplusStrong  string2;        //xy=120,463
AudioMixer4              mixer1;         //xy=299,257
AudioMixer4              mixer2;         //xy=302,438
AudioMixer4              mixer3;         //xy=432,340
AudioOutputI2S           i2s1;           //xy=573,338
AudioConnection          patchCord1(string1, 0, mixer2, 0);
AudioConnection          patchCord2(drum1, 0, mixer1, 0);
AudioConnection          patchCord3(drum2, 0, mixer1, 1);
AudioConnection          patchCord4(drum3, 0, mixer1, 2);
AudioConnection          patchCord5(string3, 0, mixer2, 2);
AudioConnection          patchCord6(string2, 0, mixer2, 1);
AudioConnection          patchCord7(mixer1, 0, mixer3, 0);
AudioConnection          patchCord8(mixer1, 0, mixer3, 1);
AudioConnection          patchCord9(mixer2, 0, mixer3, 2);
AudioConnection          patchCord10(mixer2, 0, mixer3, 3);
AudioConnection          patchCord11(mixer3, 0, i2s1, 0);
AudioConnection          patchCord12(mixer3, 0, i2s1, 1);
AudioControlSGTL5000     audioShield;     //xy=290,671
// GUItool: end automatically generated code

// Encoders pin setup
Encoder knobOne(0, 1);
Encoder knobTwo(2, 3);

#define BUTTON1 4

Bounce bounce1 = Bounce( BUTTON1, 5 );

boolean metroOn = false;

Chrono metro;

#define NOTE_F3  174.61
#define NOTE_A3  220.00
#define NOTE_C4  261.63

void setup() {
  Serial.begin(9600);
  Serial.println("David dual encoders test 1:");
  
  pinMode(BUTTON1, INPUT);
  
  AudioMemory(10);

  // turn on the output
  audioShield.enable();
  audioShield.volume(0.5);

  setupDrums();
}

int positionOne  = -999;
int positionTwo = -999;
int counter = 0;

int sn[8] = {0,1,1,0,0,1,1,0};
int hh[8] = {0,0,0,0,0,0,0,0};
int str1[8] = {1,0,0,1,0,0,0,0};
int str2[8] = {0,1,0,0,1,0,1,0};
int str3[8] = {0,0,1,0,0,1,0,1};

int encoder1Value;
int encoder2Value;
//int encoder3Value;
//int encoder4Value;
//int encoder5Value;
//int encoder6Value;

void loop() {

  if ( bounce1.update() ) {
   if ( bounce1.read() == HIGH) {
     if ( !metroOn ) {
       metroOn = true;
     } else {
       metroOn = false;
     }
   }
  }
  
  HandleEncoders(encoder1Value, encoder2Value);

  int encoder1fill = encoder2Value ^ (encoder1Value & encoder2Value);
  
  if (metroOn) {
    if (metro.hasPassed(150)) {
      metro.restart(); // Restart the chronometer.
  
      if (counter == 7) {
        counter = 0;
      } else {
        counter++;
      }
  
      if (bitRead(encoder1Value, counter) == 1) {
        drum1.noteOn();
      }
  
      if (bitRead(encoder1fill, counter) == 1) {
        drum2.noteOn();
      }
//
//      if (sn[counter] == 1) {
//        drum2.noteOn();
//      }
  
      if (hh[counter] == 1) {
        drum3.noteOn();
      }

      if (str1[counter] == 1) {
        string1.noteOn(NOTE_F3, 0.7);
      }

      if (str2[counter] == 1) {
        string2.noteOn(NOTE_A3, 0.68);
      }

      if (str3[counter] == 1) {
        string3.noteOn(NOTE_C4, 0.78);
      }
    }
  }

  // if a character is sent from the serial monitor,
  // reset both back to zero.
  if (Serial.available()) {
    Serial.read();
    Serial.println("Reset both knobs to zero");
    knobOne.write(0);
    knobTwo.write(0);
  }
}

void HandleEncoders(int &enc1, int &enc2) {
  int newOne, newTwo;
  
  newOne = knobOne.read();
  newTwo = knobTwo.read();

  if (newOne >= 1020) {
    knobOne.write(1020);
  }

  if (newOne <= 0) {
    knobOne.write(0);
  }

  if (newTwo >= 1020) {
    knobTwo.write(1020);
  }

  if (newTwo <= 0) {
    knobTwo.write(0);
  }
  
  if (newOne != positionOne || newTwo != positionTwo ) {
    enc1 = newOne/4;
    enc2 = newTwo/4;

    Serial.print("Encoder 1: ");
    Serial.print(enc1);
    Serial.print(", Encoder 2: ");
    Serial.print(enc2);
    Serial.println();
    
    positionOne = newOne;
    positionTwo = newTwo;
  }
}

void setupDrums() {
  drum1.frequency(150);
  drum1.length(150);

//  drum4.frequency(150);
//  drum4.length(150);
  
  drum2.frequency(330);
  drum2.length(75);
  drum2.pitchMod(0.55);

//  drum5.frequency(330);
//  drum5.length(75);
//  drum5.pitchMod(0.55);
//  
  drum3.frequency(600);
  drum3.length(100);
  
//  drum6.frequency(600);
//  drum6.length(100);

//  mixer2.gain(0, 0.6);
//  mixer2.gain(1, 0.6);
//  mixer2.gain(2, 0.6);
}
