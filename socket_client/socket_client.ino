#include <ESP8266WiFi.h>
#include <Adafruit_NeoPixel.h>

#define PIN D4
#define NUM_LEDS 64
#define BRIGHTNESS 1

const char* ssid = "HWHG";
const char* password = "pulseras13";

const char* device_hostname = "QBe2";
const char* host = "192.168.100.4";
const int port = 12002;

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
      //delay(2);
    String line = client.readStringUntil(126);
    //Serial.print(line);
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

void setup() {
  pixels.setBrightness(BRIGHTNESS);
  pixels.begin();
    delay(10);
  pixels.show();
  //colorWipe(strip.Color(0, 0, 0, 255), 50); // White
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
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
}

int value = 0;

void loop() {
  //delay(5000);
  ++value;

  Serial.print("connecting to ");
  Serial.println(host);
  

  print_screen();
  /*while(client.available()){

  }
  
  Serial.println();
  Serial.println("closing connection");
  */
}

