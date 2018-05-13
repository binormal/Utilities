/*
fht_adc_serial.pde
guest openmusiclabs.com 7.7.14
example sketch for testing the fht library.
it takes in data on ADC0 (Analog0) and processes them
with the fht. the data is sent out over the serial
port at 115.2kb.
*/

// FOR ARDUINO UNO

#define LIN_OUT8 1 // use the log output function
#define FHT_N 128 // set to 256 point fht
#define THRESHOLD 30

#include <FHT.h> // include the library

void setup() {
//  Serial.begin(115200); // use the serial port
  TIMSK0 = 0; // turn off timer0 for lower jitter
  ADCSRA = 0b11100101; // set the adc to free running mode 1110 0101 (Prescaler = 32 -> sampling frequency 38 kHz -> Bin size = 150)
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0

  for (int i = 0; i <= 11; i++) {
    pinMode(i, OUTPUT);
  }
}

#define reps 10
byte zeros[12];

void uhh(int pin, int bin) {
  if (fht_lin_out8[bin] > THRESHOLD) {
    digitalWrite(pin, HIGH);
  } else {
    zeros[pin]++;

    if (zeros[pin] >= reps) {
      digitalWrite(pin, LOW);
    }
  }
}

void loop() {
  while(1) { // reduces jitter
    cli();  // UDRE interrupt slows this way down on arduino1.0
    for (int i = 0 ; i < FHT_N ; i++) { // save 256 samples
      while(!(ADCSRA & 0x10)); // wait for adc to be ready
      ADCSRA = 0b11110101; // restart adc 1111 0101
      byte m = ADCL; // fetch adc data
      byte j = ADCH;
      int k = (j << 8) | m; // form into an int
      k -= 506;    
      k -= 0x0200; // form into a signed int   
      k <<= 6; // form into a 16b signed int

      fht_input[i] = k; // put real data into bins
    }
    fht_window(); // window the data for better frequency response
    fht_reorder(); // reorder the data before doing the fht
    fht_run(); // process the data in the fht
    fht_mag_lin8(); // take the output of the fht
    sei();

//    for (byte i = 0 ; i < FHT_N/2 ; i++) {
//      Serial.print(fht_lin_out8[i]); // send out the data
//      Serial.print('\t');
//    }
//    Serial.println();

    for (int i = 0; i <= 11; i++) {
      uhh(i, i+2);
    }
  }
}
