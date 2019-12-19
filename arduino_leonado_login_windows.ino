/*
  Arduino RFID Keyboard
  This little code shows you how easy you can use a RFID Modul in combination with a Keyboard function (from Arduino Pro Micro (ATmega32U4)).
  Freely usable for all with the exception of military and other anti-social activities.
  Created 22.8.2015 by Marc-André Tragé

  Pinout for Arduino Pro Micro
  ! VCC  to RFID RC522 Modul needs 3.3v, more informations about this at line 22.
    GND  GND
    IRG  not connected
    RST  pin 6
    SDA  pin 10
    MISO pin 14
    SCK  pin 15
    MOSI pin 16
  What you need for a little RFID fun:
    +  Arduino Pro Micro and Leonardo or equal Boards with a ATmega32U4
       it can emulate via USB a keyboard or a mouse) order types like this => https://www.google.de/search?q=arduino+pro+micro
    +  RFID RC522 Modul like this => https://www.google.de/search?q=rfid+rc522
    +  if you have only 5.0v VCC on Board, use a AMS1117-3.3 like this => http://www.advanced-monolithic.com/pdf/ds1117.pdf
    +  LED Button like this => https://www.google.de/search?q=LED+Light+Tactile+Push+Button

  If you have any useful changes or improvements please let me know.
  (Not a wish (but a command)) have fun (NOW (and for the rest of your life))!
*/


#include <Keyboard.h>
#include <SPI.h>
#include <MFRC522.h>
#define SS_PIN  10
#define RST_PIN 6
MFRC522 mfrc522(SS_PIN, RST_PIN);


String PW1 = "45 08 65 76"; // tag
String PW2 = "13 90 16 cb"; // card
String RFID_Key;            // Real RFID

String RFID_Fake_PW1 = "User 1 Password";
String RFID_Fake_PW2 = "User 2 Password";



void setup() {
  Serial.begin(9600);
  // RFID Modul
  SPI.begin();
  mfrc522.PCD_Init();
}


void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String RFID = "";

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    RFID.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
    RFID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }

//  Serial.println(RFID);
  if ( RFID.substring(1) == PW1 || RFID.substring(1) == PW2 ) {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(KEY_DELETE);
    delay(500);
    Keyboard.releaseAll();
//    delay(100);
    Keyboard.print(RFID_Fake_PW1);
    Keyboard.print("\n");
  }

  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
