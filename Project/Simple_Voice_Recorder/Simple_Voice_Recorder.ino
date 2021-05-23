#include <DisplayMenu.h>

int UpButton =14;
int DownButton = 15;
int EnterButton = 16;
int BackButton = 17;


int Position = 1;
int FileIndex = 0;

DisplayMenu MyMenu(UpButton, DownButton, EnterButton, BackButton, Position, FileIndex);
void setup() {

    MyMenu.Start();
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