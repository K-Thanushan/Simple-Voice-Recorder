#include <DisplayMenu.h>

#define output 13

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
  while (bin.length()<8) {
    bin = "0"+bin;
  }
  return bin;
}

int Fs = 8000;  // sampling rate 
int sensorValue; // mic reading

int UpButton =8;
int DownButton = 9;
int EnterButton = 10;
int BackButton = 11;

int Position = 1;

DisplayMenu MyMenu(UpButton, DownButton, EnterButton, BackButton, Position);

void setup() {
    MyMenu.Start();
    pinMode(output, OUTPUT);
    pinMode(A0, INPUT);
    Serial.begin(9600);
}

void loop(){
    int UpState = digitalRead(UpButton);
    if (UpState == HIGH){       
        MyMenu.Up();
    }

    int DownState = digitalRead(DownButton);
    if (DownState == HIGH){      
        MyMenu.Down();
    }

    int EnterState = digitalRead(EnterButton);
    if (EnterState == HIGH){   
        MyMenu.Enter();
    }
   
    int BackState = digitalRead(BackButton);
    if (BackState == HIGH){       
        MyMenu.Back();
    }
    //MyMenu.ButtonPress();

    sensorValue = analogRead(A0)/4;
    String binary = to_binary(sensorValue);
    Serial.print(String(sensorValue) + "  -->  ");
    for (int i = 0; i < 8; i++){
      if (binary[i] == '0'){
        digitalWrite(output, LOW);
        Serial.print("0 ");
        } 
       else{
        digitalWrite(output , HIGH);
        Serial.print("1 ");
        }
       delay(1/(8*Fs));
      }
      Serial.println();
}
