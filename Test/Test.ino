// Adapted from USB host test example for Teensy 3.6

#include "C:/Users/Tristan/Desktop/MLS/Software/Utilities/pinout.h"
#include "USBHost_t36.h"

USBHost myusb;
MIDIDevice midi1(myusb);

//int lights[12] = {2, 3, 4, 5, 6, 7, 17, 8, 16, 9, 14, 10};

void setup()
{
//	while (!Serial) ; // wait for Arduino Serial Monitor
	Serial.println("USB Host Testing");
	myusb.begin();
	midi1.setHandleNoteOff(OnNoteOff);
	midi1.setHandleNoteOn(OnNoteOn);
	midi1.setHandleControlChange(OnControlChange);
}


void loop()
{
	myusb.Task();
	midi1.read(1);
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

void OnControlChange(byte channel, byte control, byte value)
{
	Serial.print("Control Change, ch=");
	Serial.print(channel);
	Serial.print(", control=");
	Serial.print(control);
	Serial.print(", value=");
	Serial.print(value);
	Serial.println();
}

