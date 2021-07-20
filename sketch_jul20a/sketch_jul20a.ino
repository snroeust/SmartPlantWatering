
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
}

// the loop routine runs over and over again forever:
void loop() {
  // enter "A" to get Data 
   if (Serial.available() > 0) {
    // read the incoming byte:
    byte incomingByte = Serial.read();
    if(incomingByte == 65){
      // read the input on analog pin 0:
      int sensorValue = analogRead(A0);
      // print out the value you read:
      Serial.println(sensorValue);
      Serial.flush();
      
    }
  }
}
