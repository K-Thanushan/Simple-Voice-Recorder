#include "Arduino.h"
#include "DisplayMenu.h"
#include <LiquidCrystal.h>

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


void DisplayMenu::MenuPosition(int a, String Displaytext1){
    _Position = a;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(Displaytext1);
    delay(500);
}


void DisplayMenu::Start(){
    lcd.begin(16,2);
    MenuPosition(1, "Record Voice");
    delay(500);
}

void DisplayMenu::Up(){
    if (_Position==1){
        MenuPosition(4, "Reset");
    }
    else if (_Position==2){
        MenuPosition(1, "Record Voice");
    }
    else if (_Position==3){
        MenuPosition(2, "Recordings");
    }
    else if (_Position==4){
        MenuPosition(3, "Voice Characters");
    }
    else if (_Position==5){
        MenuPosition(5, "Start Recording");
    }
    else if (_Position==14){
        MenuPosition(5, "Start Recording");
    }
    else if (_Position==6){
        MenuPosition(14, "Pause Recording");
    }
    else if (_Position==7){
        MenuPosition(7, "Play Recordings");
    }
    else if (_Position==8){
        MenuPosition(7, "Play Recordings");
    }
    else if (_Position==9){
        MenuPosition(9, "Rename Recording");
    }
    else if (_Position==10){
        MenuPosition(9, "Rename Recording");
    }
    else if (_Position==11){
        MenuPosition(11, "Change Pitch");
    }
    else if (_Position==12){
        MenuPosition(11, "Change Pitch");
    }
    else if (_Position==13){
        MenuPosition(12, "Fast Forward");
    }
}

void DisplayMenu::Down(){
    if (_Position==1){
        MenuPosition(2, "Recordings");
    }
    else if (_Position==2){
        MenuPosition(3, "Voice Characters");
    }
    else if (_Position==3){
        MenuPosition(4, "Reset");
    }
    else if (_Position==4){
        MenuPosition(1, "Record Voice");
    }
    else if (_Position==5){
        MenuPosition(14, "Pause Recording");
    }
    else if (_Position==14){
        MenuPosition(6, "Stop Recording");
    }
    else if (_Position==6){
        MenuPosition(6, "Stop Recording");
    }
    else if (_Position==7){
        MenuPosition(8, "Options");
    }
    else if (_Position==8){
        MenuPosition(8, "Options");
    }
    else if (_Position==9){
        MenuPosition(10, "Delete Recording");
    }
    else if (_Position==10){
        MenuPosition(10, "Delete Recording");
    }
    else if (_Position==11){
        MenuPosition(12, "Include Echo");
    }
    else if (_Position==12){
        MenuPosition(13, "Fast Forward");
    }
    else if (_Position==13){
        MenuPosition(13, "Fast Forward");
    }
}

void DisplayMenu::Enter(){
    if (_Position==1){
        MenuPosition(5, "Start Recording");
    }
    else if (_Position==2){
        MenuPosition(7, "Play Recordings");
    }
    else if (_Position==3){
        MenuPosition(11, "Change Pitch");
    }
    else if (_Position==5){
        MenuPosition(14, "Pause Recording");
    }
    else if (_Position==8){
        MenuPosition(9, "Rename Recording");
    }
}

void DisplayMenu::Back(){            
  if (_Position == 1 or _Position ==2 or _Position == 3 or _Position == 4) {           
        MenuPosition(1, "Record Voice");
  }
  else if (_Position == 5 or _Position == 6 or _Position == 14){
        MenuPosition(1, "Record Voice");
  }
  else if (_Position == 7 or _Position == 8){
        MenuPosition(2, "Recordings");
  } 
  else if (_Position == 9 or _Position == 10){
        MenuPosition(8, "Options");
  }
  else if (_Position == 11 or _Position == 12 or _Position == 13){
        MenuPosition(3, "Voice Characters");
  }
}

//void DisplayMenu::ButtonPress(){
    
//}
