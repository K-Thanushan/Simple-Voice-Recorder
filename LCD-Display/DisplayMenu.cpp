#include "Arduino.h"
#include "DisplayMenu.h"
#include <LiquidCrystal.h>
#include <string.h>

LiquidCrystal lcd(12, 3, 4, 5, 6, 7);


DisplayMenu::DisplayMenu(int UpButton, int DownButton, int EnterButton, int BackButton, int Position)
{
    pinMode(EnterButton, INPUT); 
    pinMode(BackButton, INPUT);
    pinMode(DownButton, INPUT);
    pinMode(UpButton, INPUT);

    int _enter = EnterButton;         
    int _back = BackButton;
    int _down = DownButton;
    int _up = UpButton;
    int _Position = Position;   
}

String to_binary(int num) {
    /*
     * returns the binary value of a given decimal number between 0 and 255
     * as a 8 bit string
    */

    String bin;

    while (num) {
        bin = String(num % 2) + bin;
        num /= 2;
    }
    while (bin.length() < 8) {
        bin = "0" + bin;
    }
    return bin;
}
int output = 13; // output pin
int Fs = 8000;  // sampling rate 
int sensorValue;
String stream;
void DisplayMenu::input(){
    while (_Position == 6){
        sensorValue = analogRead(A0) / 4;
        String binary = to_binary(sensorValue);
        stream += binary;
        Serial.print(String(sensorValue) + "  -->  ");
        for (int i = 0; i < 8; i++) {
            if (binary[i] == '0') {
                digitalWrite(output, LOW);
                Serial.print("0 ");
            }
            else {
                digitalWrite(output, HIGH);
                Serial.print("1 ");
            }
            delay(1 / (8 * Fs));
        }
        Serial.println();
    }
    Serial.println(stream);
}

void DisplayMenu::Position_1(){
  _Position = 1;
  lcd.clear();
  lcd.print("Record Voice");
  delay(500);
}

void DisplayMenu::Position_2(){
  _Position = 2;
  lcd.clear();
  lcd.print("Recordings");
  delay(500);
}

void DisplayMenu::Position_3(){
  _Position = 3;
  lcd.clear();
  lcd.print("Voice Characters");
  delay(500);
}

void DisplayMenu::Position_4(){
  _Position = 4;
  lcd.clear();
  lcd.print("Reset");
  delay(500);
}

void DisplayMenu::Position_5(){
  _Position = 5;
  lcd.clear();
  lcd.print("Start Recording");
  delay(500);
}

void DisplayMenu::Position_6(){
  _Position = 6;
  lcd.clear();
  lcd.print("Stop Recording");
  delay(500);
}

void DisplayMenu::Position_7(){
  _Position = 7;
  lcd.clear();
  lcd.print("Play Recordings");
  delay(500);
}
void DisplayMenu::Position_8(){
  _Position = 8;
  lcd.clear();
  lcd.print("Options");
  delay(500);
}

void DisplayMenu::Position_9(){
  _Position = 9;
  lcd.clear();
  lcd.print("Rename Recording");
  delay(500);
}

void DisplayMenu::Position_10(){
  _Position = 10;
  lcd.clear();
  lcd.print("Delete Recording");
  delay(500);
}

void DisplayMenu::Position_11(){
  _Position = 11;
  lcd.clear();
  lcd.print("Change Pitch");
  delay(500);
}

void DisplayMenu::Position_12(){
  _Position = 12;
  lcd.clear();
  lcd.print("Include Echo");
  delay(500);
}

void DisplayMenu::Position_13(){
  _Position = 13;
  lcd.clear();
  lcd.print("Fast Forward");
  delay(500);
}

void DisplayMenu::Position_14(){
  _Position = 14;
  lcd.clear();
  lcd.print("Pause Recording");
  delay(500);
}

void DisplayMenu::Start(){
    lcd.begin(16,2);
    Position_1();
    delay(500);
}

void DisplayMenu::Up(){
    if (_Position==1){
        Position_4();
    }
    else if (_Position==2){
        Position_1();
    }
    else if (_Position==3){
        Position_2();
    }
    else if (_Position==4){
        Position_3();
    }
    else if (_Position==5){
        Position_5();
    }
    else if (_Position==14){
        Position_5();
    }
    else if (_Position==6){
        Position_14();
    }
    else if (_Position==7){
        Position_7();
    }
    else if (_Position==8){
        Position_7();
    }
    else if (_Position==9){
        Position_9();
    }
    else if (_Position==10){
        Position_9();
    }
    else if (_Position==11){
        Position_11();
    }
    else if (_Position==12){
        Position_11();
    }
    else if (_Position==13){
        Position_12();
    }
}

void DisplayMenu::Down(){
    if (_Position==1){
        Position_2();
    }
    else if (_Position==2){
        Position_3();
    }
    else if (_Position==3){
        Position_4();
    }
    else if (_Position==4){
        Position_1();
    }
    else if (_Position==5){
        Position_14();
    }
    else if (_Position==14){
        Position_6();
    }
    else if (_Position==6){
        Position_6();
    }
    else if (_Position==7){
        Position_8();
    }
    else if (_Position==8){
        Position_8();
    }
    else if (_Position==9){
        Position_10();
    }
    else if (_Position==10){
        Position_10();
    }
    else if (_Position==11){
        Position_12();
    }
    else if (_Position==12){
        Position_13();
    }
    else if (_Position==13){
        Position_13();
    }
}

void DisplayMenu::Enter(){
    if (_Position==1){
        Position_5();
    }
    else if (_Position==2){
        Position_7();
    }
    else if (_Position==3){
        Position_11();
    }
    else if (_Position==5){
        Position_6();
        input();
    }
    else if (_Position==8){
        Position_9();
    }
}

void DisplayMenu::Back(){            
  if (_Position == 1 or _Position ==2 or _Position == 3 or _Position == 4) {           
        Position_1();
  }
  else if (_Position == 5 or _Position == 6 or _Position == 14){
        Position_1();
  }
  else if (_Position == 7 or _Position == 8){
        Position_2();
  } 
  else if (_Position == 9 or _Position == 10){
        Position_8();
  }
  else if (_Position == 11 or _Position == 12 or _Position == 13){
        Position_3();
  }
}

//void DisplayMenu::ButtonPress(){
    
//}
