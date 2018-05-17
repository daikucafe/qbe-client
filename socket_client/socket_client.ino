#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

#define PIN D8
#define NUM_LEDS 64
#define BRIGHTNESS 10
#define QBE_ID 2 

const char* ssid = "HWHG";
const char* password = "pulseras13";

const char* device_hostname = "QBe1";
const char* host = "192.168.100.4";
const int port = 12002                            ;
int cycle_counter = 0;
int node_left_id = 0;
int check_num = 0;
int future_value = 0;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
WiFiClient client;

void socket_setup(){
   if (!client.connect(host, port)) {
    Serial.println("connection failed");
    return;
  }
}

void print_screen(){
    client.print("02");
    delay(2);
    String line = client.readStringUntil(126);
   /* Serial.print("MSG : ");               
    Serial.println(line); */
    int i;
    int pixel =0;
    bool start = false;

    for (i = 0 ; pixel < 64 && i < line.length()  ; ++i){
      
      if (start){
    
         pixels.setPixelColor(pixel, pixels.Color( (int) line.charAt(i)  , (int) line.charAt(i+1)  ,(int) line.charAt(i+2)  ) ) ;


       
        ++pixel;
        i=i+2;
        }
      else if ((int) line.charAt(i) == 63){
          start=!start;
        }
  
      }
      pixels.show();

      //socket_setup();
       
   
  
}

void send_left_node(int node_id){
  // return;
    char left[8];
    sprintf(left,"03%i",node_id);
    Serial.print("SE ESTA ENVIANDO");
    Serial.println(left);
    client.print(left);
    delay(2);
    //String line = client.readStringUntil(126);
}



int read_left_node(){
   int incomingByte;
    if(Serial.available()){
    size_t len = Serial.available();
    incomingByte = Serial.read();
   Serial.end();
   Serial.begin(4800);
  //for (int i = 0; i< len ;++i)
   // Serial.read();
    
  
    Serial.print("I received: ");
    Serial.print(incomingByte, DEC);
        Serial.println(node_left_id, DEC);
  }
  else {
    incomingByte=0;
    
    }

 return incomingByte;

  }

void serial_send_id(){

    if(cycle_counter % 65535/10 == 0){
    Serial1.write(QBE_ID);
    //char msg[10];
    Serial.print("Sent id ");
    Serial.println(QBE_ID, DEC);

    cycle_counter = 0;
  }
  cycle_counter++;
  
  }
  

  

void setup() {
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

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.hostname(device_hostname);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  socket_setup();
  send_left_node(0);
}

int value = 0;

void loop() {
  //delay(5000);
  ++value;

  //Serial.print("connecting to ");
  //Serial.println(host);
  

  print_screen();
 // return;
                                 
  serial_send_id();
  int new_node_left_id = read_left_node();
      Serial.print("0Mensaje a comparar  ");
       Serial.print(node_left_id, DEC);
        Serial.println(new_node_left_id, DEC);
  if(new_node_left_id ==0){
      new_node_left_id = node_left_id;
     ++  check_num;
  }
  if (check_num >= 50){
    new_node_left_id = 0;
       check_num = 0;
 
    } 

        Serial.print("1Mensaje a comparar  ");
       Serial.print(node_left_id, DEC);
        Serial.println(new_node_left_id, DEC);

        
  if ( new_node_left_id != node_left_id &&  0<= new_node_left_id  && new_node_left_id < 7){
   check_num = 0;
   node_left_id = new_node_left_id;
   
    send_left_node(node_left_id);
    }

  
  /*while(client.available()){

  }
  
  Serial.println();
  Serial.println("closing connection");
  */
}

