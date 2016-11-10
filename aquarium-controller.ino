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

//int lichtsterkte;
byte waarde;

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


