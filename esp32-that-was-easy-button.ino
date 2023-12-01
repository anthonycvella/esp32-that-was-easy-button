/*************************************************************
 Name:      ESP32 That Was Easy Button
 Purpose:   Plays a random audio sample on a ESP32 chip
 Author:    Anthony Vella
 Created:   09-04-2023
************************************************************** 
MIT License

Copyright (c) 2023 Anthony Vella

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
************************************************************** 
Samples were tuned, configured and exported as follows:
1.  In Audacity, Resample to `11025Hz`
2.  In Audacity, Export Audio as Format `WAV`, Channel `Mono`,
    Sample Rate `11025Hz` and `Encoding 8-bit PCM`
3.  In https://www.hexed.it, IMPORT WAV, select all HEX,
    `Export selected bytes as code snippet...` with option Style
    set to `C`
************************************************************** 
*/ 

#include "XT_DAC_Audio.h"

#include "Sample1.h"
#include "Sample2.h"
#include "Sample3.h"
#include "Sample4.h"

// Configure instances of each sample object to be returned later
XT_Wav_Class Sample1(sample1);
XT_Wav_Class Sample2(sample2);
XT_Wav_Class Sample3(sample3);
XT_Wav_Class Sample4(sample4);

// Instance of our Audio player configured a GPI 25 for audio output
XT_DAC_Audio_Class DacAudio(25,0);

// Configure all eligible WAV files here
int samples[] = {
  0, 1, 2, 3
};

// Constants for GPI Pins
int ledPin = 2;
int buttonPin = 21;

// Boolean for tracking the active state of the button
int buttonState = 0;

void setup() {
  // Start a serial connection so we can write to serialfor debugging
  Serial.begin(115200);

  // Configures the GPI of ledPin to be of type OUTPUT
  pinMode(ledPin, OUTPUT);

  // Configures the GPI of buttonPin to be of type INPUT
  pinMode(buttonPin, INPUT);

  // Configure the volume of our audio player to 50%
  DacAudio.DacVolume = 50;
}

void loop() {
  // Fill the buffer for the DAC to play the audio
  DacAudio.FillBuffer();

  // Constantly be reading our button state
  buttonState = digitalRead(buttonPin);

  // Change onboard LED on or off depending if the button is pressed. Button pressed turns LED on.
  if (buttonState == LOW) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // When the button is pressed and audio isn't playing, play audio
  if (buttonState == LOW && (Sample1.Playing == false &&
                             Sample2.Playing == false &&
                             Sample3.Playing == false &&
                             Sample4.Playing == false)) {
    Serial.println("Playing Audio");

    long sampleToPlay = randomSample();

    switch (sampleToPlay) {
      case 0:
        DacAudio.Play(&Sample1);
        break;
      case 1:
        DacAudio.Play(&Sample2);
        break;
      case 2:
        DacAudio.Play(&Sample3);
        break;
      case 3:
        DacAudio.Play(&Sample4);
        break;
    }
  }
}

// Returns a `long` indicating implicitly which sample to play
//
// Calculates the sample by randomly choosing one of the samples
// in the `samples` array.
long randomSample() {
  return random(sizeof(samples)/sizeof(samples[0]));
}