//+---------+   Panel - ESP32 pins
//|  R1 G1  |    R1   - IO25      G1   - IO26
//|  B1 GND |    B1   - IO27      GND  - GND
//|  R2 G2  |    R2   - IO21      G2   - IO22
//|  B2 GND |    B2   - IO23      GND  - GND
//|   A B   |    A    - IO12      B    - IO16
//|   C D   |    C    - IO17      D    - IO18
//| CLK LAT |    CLK  - IO15      LAT  - IO32
//|  OE GND |    OE   - IO33      GND  - GND
//+---------+


#include <WiFi.h>

#include <Adafruit_GFX.h>
#include <P3RGB64x32MatrixPanel.h>
#include <Fonts/FreeSansBold9pt7b.h>

P3RGB64x32MatrixPanel matrix;


char ssid[] = "Bphone B86s";
char pass[] = "88888888";
int oldMin, newMin, rFre;

#define TZ (7*60*60) /*JST*/

void setup() {
  Serial.begin(115200);

  Serial.print("Attempting to connect to Network named: ");
  Serial.println(ssid);                   // print the network name (SSID);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("");
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  configTime(TZ, 0, "ntp.nict.jp", "ntp.jst.mfeed.ad.jp"); // enable NTP

  matrix.begin();                           // setup the LED matrix

  matrix.setCursor(2, 0);
  matrix.setTextColor(matrix.color444(150, 0, 150));
  matrix.setFont();
  matrix.printf("Connect to wifi");
  matrix.swapBuffer();
  delay(1000);
  matrix.fillScreen(0);  
}

void loop()
{
  if (WiFi.status() != WL_CONNECTED)
    ESP.restart();

  time_t t;
  static time_t last_t;
  struct tm *tm;
  static const char* const wd[7] = {"Chu Nhat","Thu 2","Thu 3","Thu 4","Thu 5","Thu 6","Thu 7"};

  t = time(NULL);

  //if (last_t == t) return; // draw each second
  //last_t = t;
  tm = localtime(&t);
  oldMin = tm->tm_min;  
  Serial.print("Old");Serial.print(oldMin);Serial.print("New");Serial.println(newMin);  
  if(newMin != oldMin) {matrix.fillScreen(0);}     
  newMin = tm->tm_min;

  //matrix.fillScreen(0);

  matrix.setTextColor(matrix.color444(150, 0, 150)); // R,G,B
  matrix.setFont(&FreeSansBold9pt7b);
  matrix.setCursor(9, 13);
  matrix.printf("%02d:%02d", tm->tm_hour, tm->tm_min);
  matrix.swapBuffer();
  matrix.setFont();
  //matrix.printf(":%02d", tm->tm_sec);



  matrix.setCursor(2, 16);
  matrix.setTextColor(matrix.color444(0, 0, 150));
  matrix.printf("%s", wd[tm->tm_wday]);

  matrix.setCursor(2, 24);
  matrix.setTextColor(matrix.color444(150, 150, 150));
  matrix.printf("%02d/%02d/%04d\n", tm->tm_mday, tm->tm_mon+1, tm->tm_year+1900);
  


}
