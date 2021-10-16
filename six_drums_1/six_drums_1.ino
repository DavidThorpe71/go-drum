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
Encoder knobOne(0, 1);
Encoder knobTwo(2, 3);
//Encoder enc3(5, 6);
//Encoder enc4(7, 8);
//Encoder enc5(9, 10);
//Encoder enc6(11, 12);


#define BUTTON1 4
//#define BUTTON2 13
//#define BUTTON3 14
//#define BUTTON4 15
//#define BUTTON5 16
//#define BUTTON6 17

Bounce bounce1 = Bounce( BUTTON1, 5 );
//Bounce bounce2 = Bounce( BUTTON2, 5 );
//Bounce bounce3 = Bounce( BUTTON3, 5 );
//Bounce bounce4 = Bounce( BUTTON4, 5 );
//Bounce bounce5 = Bounce( BUTTON5, 5 );
//Bounce bounce6 = Bounce( BUTTON6, 5 );

boolean metroOn = false;

Chrono metro;

void setup() {
  Serial.begin(9600);
  Serial.println("David bit drum machine");

  pinMode(BUTTON1, INPUT);

  AudioMemory(10);

  // turn on the output
  audioShield.enable();
  audioShield.volume(0.5);

  setupDrums();
  knobTwo.write(600);
}

int positionOne = -999;
int positionTwo = -999;
//int initialEncVal3 = -999;
//int initialEncVal4 = -999;
//int initialEncVal5 = -999;
//int initialEncVal6 = -999;

int counter = 0;

//int mockBd = 199;
int mockSn = 65;
int mockHh = 200;
int mockBdFill = 10;
int mockSnFill = 4;
int mockHhFill = 67;

int bdEncValue;
int snEncValue;
//int hhEncValue;
//int bdFillEncValue;
//int snFillEncValue;
//int hhFillEncValue;

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

  HandleEncoders(bdEncValue, snEncValue);

//  int bdFill = bdFillEncValue ^ (bdEncValue & bdFillEncValue);
//  int snFill = snFillEncValue ^ (snEncValue & snFillEncValue);
//  int hhFill = hhFillEncValue ^ (hhEncValue & hhFillEncValue);

  int bdFill = mockBdFill ^ (bdEncValue & mockBdFill);
  int snFill = mockSnFill ^ (snEncValue & mockSnFill);
  int hhFill = mockHhFill ^ (mockHh & mockHhFill);

  if (metroOn) {
    if (metro.hasPassed(snEncValue)) {
      metro.restart(); // Restart the chronometer.

      if (counter == 7) {
        counter = 0;
      } else {
        counter++;
      }

      // Bass drums
      if (bitRead(bdEncValue, counter) == 1) {
        drum1.noteOn();
      }

      if (bitRead(bdFill, counter) == 1) {
        drum4.noteOn();
      }

      // Snares
      if (bitRead(mockSn, counter) == 1) {
        drum2.noteOn();
      }

      if (bitRead(snFill, counter) == 1) {
        drum5.noteOn();
      }

      // HiHats
      if (bitRead(mockHh, counter) == 1) {
        drum3.noteOn();
      }

      if (bitRead(hhFill, counter) == 1) {
        drum6.noteOn();
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
//  int newOne, newTwo, newThree, newFour, newFive, newSix;

  newOne = knobOne.read();
  newTwo = knobTwo.read();
//  newThree = enc3.read();
//  newFour = enc4.read();
//  newFive = enc5.read();
//  newSix = enc6.read();

  if (newOne >= 1020) {
    knobOne.write(1020);
  }

  if (newOne <= 0) {
    knobOne.write(0);
  }


  if (newOne != positionOne || newTwo != positionTwo ) {
    enc1 = newOne/4;
    enc2 = newTwo/4;
    Serial.print("1: ");
    Serial.print(enc1);
    Serial.print(", 2: ");
    Serial.print(enc2);
    Serial.println();

    positionOne = newOne;
    positionTwo = newTwo;
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

  mixer2.gain(0, 0.4);
  mixer2.gain(1, 0.4);
  mixer2.gain(2, 0.4);
}
