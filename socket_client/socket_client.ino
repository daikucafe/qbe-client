#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>


#define PIN D8
#define NUM_LEDS 64
#define BRIGHTNESS 10 
#define QBE_ID 1                                                                                 
#define buttonPin D7

const char* ssid = "QBe";
const char* password = "qbeqbeqbe";

const char* device_hostname = "QBe1";
const char* host = "192.168.43.3";
const int port = 12000 +  QBE_ID  ;
int cycle_counter = 0;
int node_left_id = 0;
int check_num = 0;
int future_value = 0;
int reset_num = 0;
int reset_num_1 = 0;

int ERROR_LED[] = { 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 ,  0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 ,  0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 ,  0, 0, 0 , 255, 238, 6 , 255, 238, 6 , 255, 238, 6 , 255, 238, 6 , 0, 0, 0 , 255, 238, 6 , 0, 0, 0 ,  0, 0, 0 , 255, 238, 6 , 255, 238, 6 , 255, 238, 6 , 255, 238, 6 , 0, 0, 0 , 255, 238, 6 , 0, 0, 0 ,  0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 ,  0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 ,  0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 , 0, 0, 0 };

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
WiFiClient client;

void RESET(){



  }

void ERROR_SOCKET(){

int pos = 0;
 for (int pixel = 0 ; pixel <64  ; ++pixel) {
  
      pixels.setPixelColor(pixel, pixels.Color( ERROR_LED[pos],ERROR_LED[pos+1],ERROR_LED[pos+2]  ) ) ;
      pos=pos+3;
 
    }
     pixels.show();

  }
void socket_setup() {
  if (!client.connect(host, port)) {
    // Serial.println("connection failed");
    return;
  }
}

void CHECK(){
   if (!client.connected()) {
    // Serial.println();
    // Serial.println("disconnecting.");
    client.stop();
    socket_setup();
     ERROR_SOCKET();
  
  }
  
  }

void print_screen() {
  return;
  CHECK();
  client.print("02");
  delay(2);
  String line = client.readStringUntil(126);
  /* // Serial.print("MSG : ");
    // Serial.println(line); */
  int i;
  int pixel = 0;
  bool start = false;

  for (i = 0 ; pixel < 64 && i < line.length()  ; ++i) {

    if (start) {

      if (pixel>0)
        pixels.setPixelColor(pixel, pixels.Color( (int) line.charAt(i)  , (int) line.charAt(i + 1)  , (int) line.charAt(i + 2)  ) ) ;



      ++pixel;
      i = i + 2;
    }
    else if ((int) line.charAt(i) == 63) {
      start = !start;
    }

  }
  pixels.show();

  //socket_setup();



}

void send_left_node(int node_id) {
   // return;
  char left[8];
  sprintf(left, "03%i", node_id);
  // Serial.print("SE ESTA ENVIANDO");
  // Serial.println(left);
  client.print(left);
 // delay(2);
  //String line = client.readStringUntil(126);
}

void push_botton() {
  // return;
  char left[2];
  sprintf(left, "04");
  // Serial.print("Boton Pulsado");
  // Serial.println(left);
  client.print(left);
  delay(2);
  //String line = client.readStringUntil(126);
}



int read_left_node() {
  int incomingByte;
  if (Serial.available()) {
    size_t len = Serial.available();
    incomingByte = Serial.read();
    Serial.end();
    Serial.begin(4800);
    //for (int i = 0; i< len ;++i)
    // Serial.read();


    // Serial.print("I received: ");
    pixels.setPixelColor(0,pixels.Color(255,255,255));
    pixels.show();
    // // Serial.print(incomingByte, DEC);
    // Serial.println(node_left_id, DEC);
  }
  else {
    incomingByte = 0;
  }
  return incomingByte;

}

void serial_send_id() {
Serial.write(QBE_ID);
/*
  if (cycle_counter % 65535 / 10 == 0) {
    //Serial1.write(QBE_ID);
    Serial.write(QBE_ID);
    //char msg[10];
    // Serial.print("Sent id ");
    // Serial.println(QBE_ID, DEC);

    cycle_counter = 0;
  }
  cycle_counter++;
*/
}




void setup() {


cycle_counter = 0;
 node_left_id = 0;
 check_num = 0;
future_value = 0;
 reset_num = 0;
 reset_num_1 = 0;
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin();
  delay(10);
  pixels.show();
  
  //colorWipe(strip.Color(0, 0, 0, 255), 50); // White
  //Serial.begin(115200);
  Serial.begin(4800);
  Serial1.begin(4800);
  delay(10);

  // We start by connecting to a WiFi network

  // Serial.print("Connecting to ");
  // Serial.println(ssid);
  WiFi.hostname(device_hostname);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    ERROR_SOCKET();
    // Serial.print(".");
  }

  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());
  pinMode(buttonPin, INPUT_PULLUP);

  socket_setup();
  send_left_node(0);

}

int value = 0;
int buttonState;
int button_last = 0;
int check_button() {


  buttonState = digitalRead(buttonPin);

  //// Serial.print("AQUI!! BOTTON : ");
  //// Serial.println(buttonState);

  pinMode(buttonPin, INPUT_PULLUP);
/*
  if(buttonState ==0){
      ++reset_num_1;
     if (reset_num_1 == 500 ){
        RESET();
      }
  }
    else{
        reset_num_1=0;
      }

  */

  return buttonState;
}

void loop() {
  //delay(5000);
  ++value;

  //// Serial.print("connecting to ");
  //// Serial.println(host);

  print_screen();
  int button =  check_button();
  if (button == 0 && button != button_last) {

    push_botton();
  }
  button_last = button;




  // return;

  serial_send_id();
  int new_node_left_id = read_left_node();
  // Serial.print("0Mensaje a comparar  ");
  // Serial.print(node_left_id, DEC);
  // Serial.println(new_node_left_id, DEC);
  if (new_node_left_id == 0) {
    new_node_left_id = node_left_id;
    ++  check_num;
  }
  if (check_num >= 20) {
    new_node_left_id = 0;
    check_num = 0;

    pixels.setPixelColor(0,pixels.Color(0,0,0));
    pixels.show();

    

  }

  // Serial.print("1Mensaje a comparar  ");
  // Serial.print(node_left_id, DEC);
  // Serial.println(new_node_left_id, DEC);


  if ( new_node_left_id != node_left_id &&  0 <= new_node_left_id  && new_node_left_id < 7) {
    check_num = 0;
    node_left_id = new_node_left_id;

   // send_left_node(node_left_id);
  }


  /*while(client.available()){

    }

    // Serial.println();
    // Serial.println("closing connection");
  */
}


