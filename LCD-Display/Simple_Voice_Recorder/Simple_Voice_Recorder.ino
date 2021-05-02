#include <DisplayMenu.h>

#define output 13

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
}
