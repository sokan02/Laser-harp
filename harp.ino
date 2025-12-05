#include <MIDIUSB.h>
#include <SoftwareSerial.h>

bool DEBUG_SERIAL = 0;

int notes[24] = {
  48, 50, 52, 53, 55, 57, 59,
  60, 64, 62, 65, 67, 69, 72,
  71, 74, 76, 77, 79, 81, 83, 
  84, 86, 88
};

// --- Renamed variables ---
int stringPreviouslyTriggered[24] = {0};    
int stringToTurnOff[24] = {0};              

int LDRActualAnalogValue[25] = {0};
int stringThresholdValue[24] = {0};

int globalThreshold = 100; 
int dummyRead = 0; 

int OptoVCC1 = 4; 
int OptoVCC2 = 5;
int OptoVCC3 = 6;
int OptoVCC4 = 7;
int OptoVCC5 = 8;

int OptoGND1 = 9; 
int OptoGND2 = 10;
int OptoGND3 = 11;
int OptoGND4 = 12;
int OptoGND5 = 13;

int analog1 = 0; 
int analog2 = 1;
int analog3 = 2;
int analog4 = 3;
int analog5 = 4;

int analogPinToRead = 0; 
int midiToneOffset = 0;

int midiCode = 0; 

unsigned long previousMillis = 0;       
const long interval = 300;           

void setup()
{
  if (DEBUG_SERIAL) {
    SerialUSB.begin(9600);
  }

  pinMode(OptoVCC1, OUTPUT);
  pinMode(OptoVCC2, OUTPUT);
  pinMode(OptoVCC3, OUTPUT);
  pinMode(OptoVCC4, OUTPUT);
  pinMode(OptoVCC5, OUTPUT);

  digitalWrite(OptoVCC1, LOW);
  digitalWrite(OptoVCC2, LOW);
  digitalWrite(OptoVCC3, LOW);
  digitalWrite(OptoVCC4, LOW);
  digitalWrite(OptoVCC5, LOW);

  pinMode(OptoGND1, OUTPUT);
  pinMode(OptoGND2, OUTPUT);
  pinMode(OptoGND3, OUTPUT);
  pinMode(OptoGND4, OUTPUT);
  pinMode(OptoGND5, OUTPUT);

  digitalWrite(OptoGND1, LOW);
  digitalWrite(OptoGND2, LOW);
  digitalWrite(OptoGND3, LOW);
  digitalWrite(OptoGND4, LOW);
  digitalWrite(OptoGND5, LOW);

  delay(100);

  calibrateValues(); 
}

void calibrateValues()
{
  readAnalogValues();
  for (int i = 0; i < 24; i++)
  {
    stringThresholdValue[i] = LDRActualAnalogValue[i];
  }
}

void loop()
{
  midiToneOffset = map(512, 0, 1023, 0, 101); 

  readAnalogValues();
  processValuesAndPlay();

  if (DEBUG_SERIAL) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      for (int i = 0; i < 24; i++)
      {
        SerialUSB.print(LDRActualAnalogValue[i]);
        SerialUSB.print(",");
      }
      SerialUSB.println(" ");
      SerialUSB.println("***");
    }
  }
}

void readAnalogValues()
{
  for (int m = 0; m < 5; m++)
  {
    switch (m) {
      case 0:
        digitalWrite(OptoGND1, HIGH);
        analogPinToRead = analog1;
        break;
      case 1:
        digitalWrite(OptoGND2, HIGH);
        analogPinToRead = analog2;
        break;
      case 2:
        digitalWrite(OptoGND3, HIGH);
        analogPinToRead = analog3;
        break;
      case 3:
        digitalWrite(OptoGND4, HIGH);
        analogPinToRead = analog4;
        break;
      case 4:
        digitalWrite(OptoGND5, HIGH);
        analogPinToRead = analog5;
        break;
    }
    delay(1);
    for (int i = 0; i < 5; i++)
    {
      switch (i) {
        case 0: digitalWrite(OptoVCC1, HIGH); break;
        case 1: digitalWrite(OptoVCC2, HIGH); break;
        case 2: digitalWrite(OptoVCC3, HIGH); break;
        case 3: digitalWrite(OptoVCC4, HIGH); break;
        case 4: digitalWrite(OptoVCC5, HIGH); break;
      }
      delay(1);
      dummyRead = analogRead(analogPinToRead);
      LDRActualAnalogValue[(m * 5) + i] = analogRead(analogPinToRead);
      digitalWrite(OptoVCC1, LOW);
      digitalWrite(OptoVCC2, LOW);
      digitalWrite(OptoVCC3, LOW);
      digitalWrite(OptoVCC4, LOW);
      digitalWrite(OptoVCC5, LOW);
    }
    digitalWrite(OptoGND1, LOW);
    digitalWrite(OptoGND2, LOW);
    digitalWrite(OptoGND3, LOW);
    digitalWrite(OptoGND4, LOW);
    digitalWrite(OptoGND5, LOW);
    delay(1);
  }
}

void processValuesAndPlay()
{
  for (int i = 0; i < 24; i++)
  {
    if (LDRActualAnalogValue[i] < (stringThresholdValue[i] - globalThreshold))
    {
      if (stringPreviouslyTriggered[i] == 0)
      {
        stringPreviouslyTriggered[i] = 1;
        stringToTurnOff[i] = 1;

        noteOn(0, notes[i], 64);

        if (DEBUG_SERIAL) {
          SerialUSB.print("+ PLAYED NUMBER ");
          SerialUSB.println(i);
        }

        MidiUSB.flush();
      }
    }
    else if (LDRActualAnalogValue[i] > (stringThresholdValue[i] - globalThreshold / 5))
    {
      stringPreviouslyTriggered[i] = 0;
    }
  }

  delay(5);

  for (int i = 0; i < 24; i++)
  {
    if (stringToTurnOff[i] == 1)
    {
      stringToTurnOff[i] = 0;

      noteOff(0, notes[i], 64);

      if (DEBUG_SERIAL) {
        SerialUSB.print("# STOPPED NUMBER ");
        SerialUSB.println(i);
      }

      MidiUSB.flush();
    }
  }
}

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOnEvent = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOnEvent);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOffEvent = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOffEvent);
}

