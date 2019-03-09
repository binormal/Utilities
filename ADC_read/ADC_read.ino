// read ADC and store highest value

#include <math.h>

  const int bits = 13;
  const int zero = 490; // ADC reading with no signal
  const float res = pow(2, bits); // ADC resolution
  const float ref = 3.3;  // Reference voltage
  const float v_zero = zero / res * ref;

void setup() {
  Serial.begin(115200);
  while (!Serial);
}

void loop() {

  
  static int max = zero;
  static int min = zero;
  static float v_max_abs = v_zero;
  static float v_min_abs = v_zero;
  static float v_max_rel = 0;		// max voltage relative to v_zero
  static float v_min_rel = 0;		// min voltage relative to v_zero
  static int bin[7];

  int reading = analogRead(1);

  float v_rel = reading / res * ref - v_zero;
  if (reading > max) {
    max = reading;
    v_max_abs = max / res * ref;
    v_max_rel = v_max_abs - v_zero;
  } else if (reading < min) {
    min = reading;
    v_min_abs = min / res * ref;
    v_min_rel = v_min_abs - v_zero;
  }



  if (v_rel < 0.5) {
    bin[0]++;
  } else if (v_rel < 1.0) {
    bin[1]++;
  } else if (v_rel < 1.5) {
    bin[2]++;
  } else if (v_rel < 2.0) {
    bin[3]++;
  } else if (v_rel < 2.5) {
    bin[4]++;
  } else if (v_rel < 3.0) {
    bin[5]++;
  } else {
    bin[6]++;
  }

  Serial.print(reading);
  Serial.print('\t');
  Serial.print(min);
  Serial.print('\t');
  Serial.print(max);
  Serial.print('\t');
  Serial.print(v_min_abs);
  Serial.print('\t');
  Serial.print(v_max_abs);
  Serial.print('\t');
  Serial.print(v_min_rel);
  Serial.print('\t');
  Serial.print(v_max_rel);
  Serial.print('\t');
  Serial.print(bin[0]);
  Serial.print('\t');
  Serial.print(bin[1]);
  Serial.print('\t');
  Serial.print(bin[2]);
  Serial.print('\t');
  Serial.print(bin[3]);
  Serial.print('\t');
  Serial.print(bin[4]);
  Serial.print('\t');
  Serial.print(bin[5]);
  Serial.print('\t');
  Serial.print(bin[6]);
  Serial.print('\t');
  Serial.println(v_rel);
}
