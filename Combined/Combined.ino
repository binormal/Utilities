// #include "pinout.h"
#include <USBHost_t36.h>
#include <Audio.h>

enum sources { analog, midi };
int source = analog;

USBHost usb_host;
MIDIDevice midi_dev(usb_host);

const int pwm_resolution = 8;
const int pwm_max = 255;
const float pwm_freq = 35156.25;  // See https://pjrc.com/teensy/td_pulse.html

AudioInputAnalog adc(A1);
AudioAnalyzeFFT1024 fft;
AudioConnection patchCord(adc, fft);

const int num_lights = 12;
const int lights[num_lights] = {2, 3, 4, 5, 6, 7, 17, 8, 16, 9, 14, 10};
float level[num_lights];

void setup() {
  Serial.begin(115200);
  AudioMemory(12);
  analogWriteResolution(pwm_resolution);
  fft.windowFunction(AudioWindowHanning1024);
  for (int i = 0; i < num_lights; i++) {
//    analogWriteFrequency(lights[i], pwm_freq);
    pinMode(lights[i], OUTPUT);
  }

  usb_host.begin();

}

void loop() {
    if (Serial.available() > 0) {
        while (Serial.available() > 0) {
            Serial.read();
        }

        source = !source;

        switch(source) {
          case analog: {
            clean_midi();
          } break;

          case midi: {
            init_midi();
          } break;

          default:
            Serial.println("Unknown source");
        }
    }

    switch(source) {
      case analog: {
          if (fft.available()) {
            level[0] = fft.read(0, 1); // read() returns float from 0.0 to 1.0
            level[1] = fft.read(2, 3);
            level[2] = fft.read(4, 10);
            level[3] = fft.read(11, 15);
            level[4] = fft.read(16, 25);
            level[5] = fft.read(26, 45);
            level[6] = fft.read(46, 70);
            level[7] = fft.read(71, 95);
            level[8] = fft.read(96, 130);
            level[9] = fft.read(131, 181);
            level[10] = fft.read(182, 250);
            level[11] = fft.read(251, 350);

            Serial.print(AudioMemoryUsageMax());
            Serial.print('\t');
            for (int i = 0; i < num_lights; i++) {
              analogWrite(lights[i], level[i]*pwm_max);
        //    if (level[i] > 0) {
        //      digitalWrite(lights[i], HIGH);
        //    } else {
        //      digitalWrite(lights[i], LOW);
        //    }

              Serial.print(level[i]);
              Serial.print('\t');
            }
            Serial.println();
          }
      } break;

      case midi: {
        midi_dev.read();
      } break;

      default:
        Serial.println("Unknown source");
    }


}


void init_midi() {
  midi_dev.setHandleNoteOn(OnNoteOn);
  midi_dev.setHandleNoteOff(OnNoteOff);
  Serial.println("Initialized MIDI handlers");
}

void clean_midi() {
  midi_dev.setHandleNoteOn(EmptyFunction);
  midi_dev.setHandleNoteOff(EmptyFunction);
  Serial.println("Cleared MIDI handlers");
}

void OnNoteOn(byte channel, byte note, byte velocity)
{
	Serial.print("Note On, ch=");
	Serial.print(channel);
	Serial.print(", note=");
	Serial.print(note);
	Serial.print(", velocity=");
	Serial.print(velocity);
	Serial.println();
    int light = map(note, 21, 108, 0, 11);
    analogWrite(lights[light], velocity*2);
}

void OnNoteOff(byte channel, byte note, byte velocity)
{
	Serial.print("Note Off, ch=");
	Serial.print(channel);
	Serial.print(", note=");
	Serial.print(note);
	//Serial.print(", velocity=");
	//Serial.print(velocity);
	Serial.println();
    int light = map(note, 21, 108, 0, 11);
    analogWrite(lights[light], 0);
}

void EmptyFunction(byte, byte, byte) { }
