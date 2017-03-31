#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>


// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS     8
#define TFT_RST    6  // you can also connect this to the Arduino reset
                      // in which case, set this #define pin to 0!
#define TFT_DC     7


Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

void setup() {

  // Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  tft.fillScreen(ST7735_BLACK);//set screen to black

  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextWrap(true);
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(115200); //esp hopefully
  //delay(500);
  getResponse("AT+RST");
  getResponse("AT+CWMODE=3");
  //Serial1.println("AT+CWJAP=\"HobbitHole\",\"inthebeginningtherewerefleas\"");
  //getResponse();
  getResponse("AT+CIPMUX=1");
  getResponse("AT+CIPSERVER=1,80");

}
char w;
String text = "";
String requestType = "";
String URL = "";
String webpage = "<head>Hello!</head><body>This is ESP</body>";
void loop() {
  // put your main code here, to run repeatedly:
  //Serial1.println("AT+RST");
  //delay(1000);
  while(Serial.available()){
    char c = Serial.read();
    Serial1.write(c);
  }
  if(Serial1.available()){
    tft.setCursor(0, 0);
    tft.fillScreen(ST7735_BLACK);
    if(Serial1.find("+IPD,")){
      //getResponse("");
      Serial.print("+IPD,");
      int connectionID = Serial1.read()-48;
      tft.print(connectionID);
      Serial.print(connectionID);
      //we should have a request
      requestType = "";
      while(w != ':'){
        w = Serial1.read();
        Serial.write(w);
        tft.print(w);
      }
      w = Serial1.read();
      while(w != ' '){
        requestType += w;
        w = Serial1.read();
      }
      Serial.print(requestType);
      URL = "";
      w = Serial1.read();
      while(w != ' '){
        URL += w;
        w = Serial1.read();
      }
      Serial.print(URL);

      while(Serial1.available()){
        char v = Serial1.read();
        Serial.write(v);
        tft.print(v);
      }
      text = "AT+CIPSEND=";
      text += connectionID;
      text += ",";
      text += webpage.length();
      getResponse(text);
      getResponse(webpage);
      text = "AT+CIPCLOSE=";
      text += connectionID;
      getResponse(text);
    }
    //text = "";
    while(Serial1.available()){
      char t = Serial1.read();
      Serial.write(t);
      tft.print(t);
      //text += t;
    }
  }
}

void getResponse(String data){
  long int time = millis();
  Serial1.println(data);
  tft.setCursor(0, 0);
  tft.fillScreen(ST7735_BLACK);
  while((time + 5000) > millis()){
    if(Serial1.available()){
      while(Serial1.available()){
        char u = Serial1.read();
        tft.print(u);
        Serial.write(u);
      }
      break;
    }
  }
}
