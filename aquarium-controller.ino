
#include <Wire.h>
//use pin scl and scda on arduino uno
#include <LiquidCrystal_I2C.h>
//for storing data past boot
#include <EEPROM.h>
//for using tiny rtc
#include <Wire.h>
#include <RTClib.h>
RTC_DS1307 RTC;
#define BACKLIGHT_PIN     13
//Gebruik i2cguess om niet werkende lcd's uit te lezen
//Gebruik de library van F Malpartida voor aansturing van de LCD
LiquidCrystal_I2C lcd(0x3f, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

int photocellPin = 1;// Digital Pin the photocell is connected to is A1
int photocellReading;
int lichtsterkte;
byte waarde;
byte controller1_aanuur;
byte controller1_aanminuut;
byte controller1_uituur;
byte controller1_uitminuut;
int address = 0;
int c1_aanuur = 1;
int c1_uituur = 2;
int c2_aanuur = 3;
int c2_uituur = 4;
byte button1_state = LOW;
int button1_pin = 2;


/////////////////////////////
//SETUP
void setup()
{
  //LCD setup
pinMode ( BACKLIGHT_PIN, OUTPUT );
  digitalWrite ( BACKLIGHT_PIN, LOW );
  pinMode(button1_pin, INPUT);
  lcd.begin(16,2);               // initialize the lcd 
  lcd.home ();                   // go home
  lcd.print("Hello, ARDUINO ");  
  lcd.setCursor ( 0, 1 );        // go to the next line
  lcd.setCursor(0,0);
  delay(500);
  lcd.print (" Booting....  ");
  delay(500);
  lcd.setCursor(0,0);
  lcd.print ("             ");
  delay(500);
  lcd.setCursor(0,0);
  lcd.print("lichtsterkte -");
  //waarde van de eeprom uitlezen
  waarde = EEPROM.read(address); 
  //rtc setup
  Serial.begin(57600);
  Wire.begin();
  RTC.begin();
  //als de klok niet klopt, zet rtc.adjust dan buiten het if statement, upload, en zet rt.adjust terug in het if statement en upload. Op deze manier 
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
   /* Serial.print(now.year(), DEC);
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
  */
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
  photocellReading = analogRead(photocellPin); 
  lichtsterkte=1023-photocellReading;
  lcd.setCursor(0,1);
  lcd.print(lichtsterkte);lcd.print(" ") ;lcd.print(waarde);lcd.print(" ");lcd.print(button1_state);
  delay(500);
  lcd.setCursor(0,0);
  lcd.print(now.day());lcd.print("/");lcd.print(now.month());lcd.print("/");lcd.print(now.year());lcd.print(" ");lcd.print(now.hour());lcd.print(":");lcd.print(now.minute());
  }

