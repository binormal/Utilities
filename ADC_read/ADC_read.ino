// read ADC and store highest value

void setup() {
  Serial.begin(9600);
  while (!Serial);
}

void loop() {
  const int zero = 505;	// ADC reading with no signal
  const float res = 1024.0;	// ADC resolution
  const float ref = 3.3;	// Reference voltage
  const float v_zero = zero / res * ref;
  
  static int max = zero;
  static int min = zero;
  static float v_max_abs = v_zero;
  static float v_min_abs = v_zero;
  static float v_max_rel = 0;		// max voltage relative to v_zero
  static float v_min_rel = 0;		// min voltage relative to v_zero

  int reading = analogRead(1);

  if (reading > max) {
    max = reading;
    v_max_abs = max / res * ref;
    v_max_rel = v_max_abs - v_zero;
  } else if (reading < min) {
    min = reading;
    v_min_abs = min / res * ref;
    v_min_rel = v_min_abs - v_zero;
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
  Serial.println(v_max_rel);
}
