
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial.flush();
}

// the loop routine runs over and over again forever:
void loop() {

    int sensorValue = analogRead(A0);
    // print out the value you read:
    Serial.println(sensorValue);
    delay(5000); 
}