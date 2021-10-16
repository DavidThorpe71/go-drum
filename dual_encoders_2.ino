#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <Encoder.h>
#include <Chrono.h>
#include <Bounce.h>

// GUItool: begin automatically generated code
AudioSynthSimpleDrum     drum1;          //xy=111,234
AudioSynthSimpleDrum     drum2;          //xy=112,276
AudioSynthSimpleDrum     drum3;          //xy=113,319
AudioSynthSimpleDrum     drum4;          //xy=119,416
AudioSynthSimpleDrum     drum5;          //xy=120,459
AudioSynthSimpleDrum     drum6;          //xy=121,500
AudioMixer4              mixer1;         //xy=299,257
AudioMixer4              mixer2;         //xy=302,438
AudioMixer4              mixer3;         //xy=432,340
AudioOutputI2S           i2s1;           //xy=573,338
AudioConnection          patchCord1(drum1, 0, mixer1, 0);
AudioConnection          patchCord2(drum2, 0, mixer1, 1);
AudioConnection          patchCord3(drum3, 0, mixer1, 2);
AudioConnection          patchCord4(drum4, 0, mixer2, 0);
AudioConnection          patchCord5(drum5, 0, mixer2, 1);
AudioConnection          patchCord6(drum6, 0, mixer2, 2);
AudioConnection          patchCord7(mixer1, 0, mixer3, 0);
AudioConnection          patchCord8(mixer1, 0, mixer3, 1);
AudioConnection          patchCord9(mixer2, 0, mixer3, 2);
AudioConnection          patchCord10(mixer2, 0, mixer3, 3);
AudioConnection          patchCord11(mixer3, 0, i2s1, 0);
AudioConnection          patchCord12(mixer3, 0, i2s1, 1);
AudioControlSGTL5000     audioShield;     //xy=290,671
// GUItool: end automatically generated code

// Encoders pin setup
Encoder enc1(0, 1);
Encoder enc2(2, 3);
Encoder enc3(5, 6);
Encoder enc4(7, 8);
Encoder enc5(9, 10);
Encoder enc6(11, 12);

Encoder encArray[6] = {enc1, enc2, enc3, enc4, enc5, enc6};

#define BUTTON1 4
#define BUTTON2 13
#define BUTTON3 14
#define BUTTON4 15
#define BUTTON5 16
#define BUTTON6 17

Bounce bounce1 = Bounce( BUTTON1, 5 );
Bounce bounce2 = Bounce( BUTTON2, 5 ); 
Bounce bounce3 = Bounce( BUTTON3, 5 ); 
Bounce bounce4 = Bounce( BUTTON4, 5 ); 
Bounce bounce5 = Bounce( BUTTON5, 5 ); 
Bounce bounce6 = Bounce( BUTTON6, 5 ); 

boolean metroOn = false;

Chrono metro;

void setup() {
  Serial.begin(9600);
  Serial.println("David dual encoders test 1:");
  
  pinMode(BUTTON1, INPUT);
  
  AudioMemory(10);

  // turn on the output
  audioShield.enable();
  audioShield.volume(0.5);

  setupDrums()
}

int positionLeft  = -999;
int positionRight = -999;
int counter = 0;

int sn[8] = {0,1,1,0,0,1,1,0};
int hh[8] = {0,0,0,0,0,0,0,0};
int str1[8] = {1,0,0,1,0,0,0,0};
int str2[8] = {0,1,0,0,1,0,1,0};
int str3[8] = {0,0,1,0,0,1,0,1};

int encoder1Value;
int encoder2Value;
int encoder3Value;
int encoder4Value;
int encoder5Value;
int encoder6Value;

void loop() {

  if ( bouncer.update() ) {
   if ( bouncer.read() == HIGH) {
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
  
//      if (bitRead(encoder1fill, counter) == 1) {
//        drum2.noteOn();
//      }

      if (sn[counter] == 1) {
        drum2.noteOn();
      }
  
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
    knobLeft.write(0);
    knobRight.write(0);
  }
}

void HandleEncoders(int &enc1, int &enc2) {
  int newOne, newTwo, newThree, newFour, newFive, newSix;
  int encVals[6] = {newOne, newTwo, newThree, newFour, newFive, newSix};
  
  newOne = knobOne.read();
  newTwo = knobTwo.read();
  newThree = knobThree.read();
  newFour = knobFour.read();
  newFive = knobFive.read();
  newSix = knobSix.read();

  for (int i = 0; i <= sizeof(encArray); i++) {
    encVals[i] = encArray[i].read()
    checkInBounds(encVals[i], i);
  }
  
  if (newLeft != positionLeft || newRight != positionRight ) {
    enc1 = newLeft/4;
    enc2 = newRight/4;

    Serial.print("Encoder 1: ");
    Serial.print(enc1);
    Serial.print(", Encoder 2: ");
    Serial.print(enc2);
    Serial.println();
    
    positionLeft = newLeft;
    positionRight = newRight;
  }
}

void setupDrums() {
  drum1.frequency(150);
  drum1.length(150);

  drum4.frequency(150);
  drum4.length(150);
  
  drum2.frequency(330);
  drum2.length(75);
  drum2.pitchMod(0.55);

  drum5.frequency(330);
  drum5.length(75);
  drum5.pitchMod(0.55);
  
  drum3.frequency(600);
  drum3.length(100);
  
  drum6.frequency(600);
  drum6.length(100);

  mixer2.gain(0, 0.6);
  mixer2.gain(1, 0.6);
  mixer2.gain(2, 0.6);
}

void checkInBounds(int encVal, int item) {
  if (encVal >= 1020) {
    encArray[item].write(1020);
  }

  if (encVal <= 0) {
    encArray[item].write(0);
  }
}
