#include "Arduino.h"
#include "DisplayMenu.h"
#include <LiquidCrystal.h>
#include <SD.h>

LiquidCrystal lcd(12, 3, 4, 5, 6, 7);


DisplayMenu::DisplayMenu(int UpButton, int DownButton, int EnterButton, int BackButton, int Position, int FileIndex)
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
    int _FileIndex = FileIndex;  
}

//String nameList[] = {"Voice1", "Voice2","Voice3"}; 
int N = 2;
int Volume = 50;
String nameList[100];


void DisplayMenu::MenuPosition1(int a, String Displaytext1, String Displaytext2){
    _Position = a;
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(">");
    lcd.setCursor(1,0);
    lcd.print(Displaytext1);
    lcd.setCursor(0,1);
    lcd.print(Displaytext2);
    delay(500);
}

void DisplayMenu::MenuPosition2(int a, String Displaytext3, String Displaytext4){
    _Position = a;
    lcd.clear();
    lcd.setCursor(0,1);
    lcd.print(">");
    lcd.setCursor(1,1);
    lcd.print(Displaytext3);
    lcd.setCursor(10,1);
    lcd.print(Displaytext4);
    delay(500);
}



void DisplayMenu::Start(){
    lcd.begin(16,2);
    MenuPosition1(1,"Record Voice", "Recordings");
    delay(500);
}

void DisplayMenu::Enter(){
    if (_Position==1){
        MenuPosition1(4, "Start Recording", "");
    }
    else if (_Position==2){
        File root;
        int count = 0;
        root = SD.open("/");
  
        while (true) {
            File entry =  root.openNextFile();
            if (! entry) {
            // no more files
            break;
            }
    
            String name = entry.name();
            int len = name.length();
  
            if (name.substring(len-3, len) == "WAV"){
            //Serial.println(name);
            nameList[count] = name.substring(0, len-4);
            count ++;
            }
            entry.close();
        }
        root.close();
        N = count;
        MenuPosition1(19, nameList[_FileIndex], nameList[_FileIndex + 1]);
    }
    else if (_Position>=19){
        MenuPosition1(6, "Play Recordings", "Change Pitch");
    }
    else if (_Position==4){
        MenuPosition2(5, "Stop Recording", "");
    }
    else if (_Position==5){
        lcd.clear();
        lcd.print("Saving File");
        delay(1000);
        lcd.clear();
        lcd.print("Saved File");
        delay(1000);
        MenuPosition1(1, "Record Voice", "Recordings");
    }
    else if (_Position==6){
        MenuPosition2(14, "Pause", "Stop");
    }
    else if (_Position==14){
        MenuPosition2(16, "Stop", "Back");
    }
    else if (_Position==17){
        MenuPosition1(7, "Play Recordings", "Change Pitch");
    }
    else if (_Position==7){
        MenuPosition1(18, "Increase Volume", "Decrease Volume");
    }
    else if (_Position==8){
        MenuPosition1(12, "High Pitch", "Low Pitch");
    }
}

void DisplayMenu::Up(){
    if (_Position==1){
        MenuPosition1(3, "Factory Reset", "Record Voice");
    }
    else if (_Position==2){
        MenuPosition1(1, "Record Voice", "Recordings");
    }
    else if (_Position==3){
        MenuPosition1(2, "Recordings", "Factory Reset");
    }
    else if (_Position==4){
        MenuPosition1(4, "Start Recording", "");
    }
    else if (_Position==5){
        MenuPosition2(5, "Stop Recording", "");
    }
    else if (_Position==6){
        MenuPosition1(6, "Play Recording", "Change Volume");
    }
    else if (_Position==7){
        MenuPosition1(6, "Play Recording", "Change Volume");
    }
    else if (_Position==8){
        MenuPosition1(7, "Change Volume", "Change Pitch");
    }
    else if (_Position==9){
        MenuPosition1(8, "Change Pitch", "Include Echo");
    }
    else if (_Position==10){
        MenuPosition1(9, "Include Echo", "Fast Forward");
    }
    else if (_Position==11){
        MenuPosition1(10, "Fast Forward", "DeleteRecording");
    }
    else if (_Position==12){
        MenuPosition1(12, "High Pitch", "Low Pitch");
    }
    else if (_Position==13){
        MenuPosition1(12, "High Pitch", "Low Pitch");
    }
    else if (_Position==14){
        MenuPosition2(14, "Pause", "Stop");
    }
    else if (_Position==15){
        MenuPosition2(14, "Pause", "Stop");
    }
    else if (_Position==16){
        MenuPosition2(16, "Stop", "Back");
    }
    else if (_Position==17){
        MenuPosition2(16, "Stop", "Back");
    }
    else if (_Position==18){
        Volume++;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Volume:");
        lcd.setCursor(9,0);
        lcd.print(Volume);
        delay(2000);
        MenuPosition1(18, "Increase Volume", "Decrease Volume");
    }
    else if (_Position==19){
        _FileIndex = _FileIndex;
        MenuPosition1(18, nameList[_FileIndex], nameList[_FileIndex + 1]);
    }
    else if (_Position>19){
        _FileIndex --;
        _Position--;
        MenuPosition1(_Position, nameList[_FileIndex], nameList[_FileIndex + 1]);
    }
}

void DisplayMenu::Down(){
    if (_Position==1){
        MenuPosition1(2, "Recordings", "Factory Reset");
    }
    else if (_Position==2){
        MenuPosition1(3, "Factory Reset", "Record Voice");
    }
    else if (_Position==3){
        MenuPosition1(1, "Record Voice", "Recordings");
    }
    else if (_Position==4){
        MenuPosition1(4, "Start Recording", "");
    }
    else if (_Position==5){
        MenuPosition2(5, "Stop Recording", "");
    }
    else if (_Position==6){
        MenuPosition1(7, "Change Volume", "Change Pitch");
    }
    else if (_Position==7){
        MenuPosition1(8, "Change Pitch", "Include Echo");
    }
    else if (_Position==8){
        MenuPosition1(9, "Include Echo", "Fast Forward");
    }
    else if (_Position==9){
        MenuPosition1(10, "Fast Forward", "DeleteRecording");
    }
    else if (_Position==10){
        MenuPosition1(11, "DeleteRecording", "");
    }
    else if (_Position==11){
        MenuPosition1(11, "DeleteRecording", "");
    }
    else if (_Position==12){
        MenuPosition1(13, "Low Pitch", "");
    }
    else if (_Position==13){
        MenuPosition1(13, "Low Pitch", "");
    }
    else if (_Position==14){
        MenuPosition2(15, "Stop", "");
    }
    else if (_Position==15){
        MenuPosition2(15, "Stop", "");
    }
    else if (_Position==16){
        MenuPosition2(17, "Back","");
    }
    else if (_Position==17){
        MenuPosition2(17, "Back","");
    }
    else if (_Position==18){
        Volume--;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Volume:");
        lcd.setCursor(9,0);
        lcd.print(Volume);
        delay(2000);
        MenuPosition1(18, "Increase Volume", "Decrease Volume");
    }
    else if (_Position>=19){
        _FileIndex++;
        _Position++;
        if (_FileIndex == N){
            MenuPosition1(_Position, nameList[_FileIndex], "");
        }
        MenuPosition1(_Position, nameList[_FileIndex], nameList[_FileIndex + 1]);
    }
 }



void DisplayMenu::Back(){   
  if (_Position == 1 or _Position ==2 or _Position == 3) {           
        MenuPosition1(1, "Record Voice", "Recordings");
  }
  else if (_Position == 4 or _Position == 5){
        MenuPosition1(1, "Record Voice", "Recordings");
  }
  else if (_Position>=18){
        MenuPosition1(2, "Recordings", "Factory Reset");
  }
  else if (_Position==17){
        MenuPosition1(7, "Play Recordings", "Change Pitch");
  }
  else if (_Position == 7 or _Position == 8 or _Position == 9 or _Position == 10 or _Position == 11){
        MenuPosition1(18, nameList[_FileIndex], nameList[_FileIndex + 1]);
  } 
  else if (_Position == 12 or _Position == 13){
        MenuPosition1(8, "Change Pitch", "Include Echo");
  }
  else if (_Position == 18){
        MenuPosition1(7, "ChangeVolume", "Change Pitch");
  }
}