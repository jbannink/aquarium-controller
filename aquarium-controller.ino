// IMPORTANT: Adafruit_TFTLCD LIBRARY MUST BE SPECIFICALLY
// CONFIGURED FOR EITHER THE TFT SHIELD OR THE BREAKOUT BOARD.
// SEE RELEVANT COMMENTS IN Adafruit_TFTLCD.h FOR SETUP.

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
#include <Wire.h>
//use pin scl and scda on arduino uno
//#include <LiquidCrystal_I2C.h>
//for storing data past boot
#include <EEPROM.h>
//for using tiny rtc
#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 RTC;
#define BACKLIGHT_PIN     13
//Gebruik i2cguess om niet werkende lcd's uit te lezen
//Gebruik de library van F Malpartida voor aansturing van de LCD
//LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
//tft-settings

#define LCD_CS A3 // Chip Select goes to uintAnalog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0
#define LCD_RESET A4 // Can alternately just connect to Arduino's reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:
// Adafruit_TFTLCD tft;

//int lichtsterkte;
byte waarde;
//kloksettings
int address = 0;
int c1_aanuur = 19;
int c1_aanminuut =15;
int c1_uituur = 19;
int c1_uitminuut = 10;
int c2_aanuur = 3;
int c2_uituur = 4;
byte button1_state = LOW;
int button1_pin = 2;
const int c1_redPin = 44;
const int c1_greenPin = 45;
const int c1_bluePin = 46;
int uurnu;
int minuutnu;
int secnu;

/////////////////////////////
//SETUP
void setup()
{
  tft.reset();
  uint16_t identifier = 0x9341;
// pinMode(c1_redPin,OUTPUT);
// pinMode(c1_greenPin,OUTPUT);
// pinMode(c1_bluePin,OUTPUT);
  //waarde van de eeprom uitlezen
  waarde = EEPROM.read(address); 
  //rtc setup
  Serial.begin(57600);
  Wire.begin();
  RTC.begin();
  //RTC.adjust(DateTime(__DATE__, __TIME__));
    //als de klok niet klopt, zet rtc.adjust dan buiten het if statement, upload, en zet rt.adjust terug in het if statement en upload.
  //Op deze manier wordt de tijd gezet met de tijd dat het programma gecompileerd werd
  if (! RTC.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    RTC.adjust(DateTime(__DATE__, __TIME__));
  }
  tft.begin(identifier);

  Serial.println(F("Benchmark                Time (microseconds)"));

  Serial.print(F("Screen fill              "));
  Serial.println(testFillScreen());
  delay(500);

 }

////////////////////////////
//LOOP
void loop()
  {
    //rtc function
    DateTime now = RTC.now();
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.print(" since 1970 = ");
    Serial.print(now.unixtime());
    Serial.print("s = ");
    Serial.print(now.unixtime() / 86400L);
    Serial.println("d");//End rtc function
  // testFillScreen();
    tft.setCursor(0,0);
    tft.setTextColor(WHITE);
     tft.println("Datum");
    tft.print(now.day(), DEC);tft.print("/");tft.print(now.month(),DEC);tft.print("/");tft.println(now.day(),DEC);
    tft.print(now.hour(), DEC);tft.print(":");tft.println(now.minute(), DEC);
  //read buttons en wijzig waarde 0-23
  button1_state = digitalRead(button1_pin);
  if (button1_state == HIGH)
     {
     waarde=waarde+1;
     if (waarde == 24) 
        {
        waarde=0;
        }
     }
  //gewijzigde waarde opslaan in eeprom   
  EEPROM.write(address, waarde);   
  //  lichtsterkte uitlezen
  digitalWrite ( BACKLIGHT_PIN, LOW );
  uurnu=now.hour();
  minuutnu=now.minute();
  secnu=now.second();
  Serial.print("uurnu = ");
  Serial.print(uurnu);
if ((uurnu == c1_aanuur) && (minuutnu == c1_aanminuut) && (secnu <=2))
  {
  sunup();
  }
if ((uurnu == c1_uituur) && (minuutnu == c1_uitminuut) && (secnu <= 2)) 
   {
    sundown();
   }
}

void sunup() {
  for(int fadeValue=0; fadeValue<=254; fadeValue++) {
    Serial.print("sunup");
  digitalWrite(c1_greenPin, fadeValue);
  digitalWrite(c1_redPin, fadeValue);
  digitalWrite(c1_bluePin, fadeValue);
  delay(50);
  }
}
void sundown() {
   Serial.print("sundown");
   delay (3000);
   for(int fadeValue=0; fadeValue<=254; fadeValue++) {
    Serial.print("sundown");
    digitalWrite(c1_greenPin, 254-fadeValue);
    digitalWrite(c1_redPin, 254-fadeValue);
    digitalWrite(c1_bluePin, 254-fadeValue);
    delay(50);
    }
}

unsigned long testFillScreen() {
 // unsigned long start = micros();
  tft.fillScreen(BLACK);
  tft.fillScreen(RED);
  tft.fillScreen(GREEN);
  tft.fillScreen(BLUE);
  tft.fillScreen(BLACK);
 // return micros() - start;
}
