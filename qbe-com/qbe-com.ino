
#define QBE_ID 1

int cycle_counter = 0;

void setup() {
  Serial.begin(4800);
  Serial1.begin(4800);
 
}

void loop() {

  //check UART for data
  if(Serial.available()){
    size_t len = Serial.available();
    int incomingByte = Serial.read();
    Serial.print("I received: ");
    Serial.println(incomingByte, DEC);
  }

  // Sending ID to the right node
  if(cycle_counter % 65535 == 0){
    Serial1.write(QBE_ID);
    //char msg[10];
    Serial.print("Sent id ");
    Serial.println(QBE_ID, DEC);
    cycle_counter = 0;
  }
  cycle_counter++;
}
