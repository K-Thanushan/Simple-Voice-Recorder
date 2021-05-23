#include "Arduino.h"
#include "DisplayMenu.h"
#include <SPI.h>
#include <SD.h>
#include <TMRpcm.h>

/// I2C activation for physical implementation 

 /* #include<Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);   */            /// also do changes in 'start' function
                                           
 /// For simulation
 
 #include <LiquidCrystal.h>
LiquidCrystal lcd(12, 3, 4, 5, 6, 7);   
 
#define SD_CSPin 53

TMRpcm audio;
File myFile, root;

char filePrefixname[50] = "aud";
char exten[10] = ".wav";
char file_name[50];
int file_number = 0;

int input_signal = A0; 
int audio_output = 11;
int Fs = 11025;

volatile char fileSlNum[20] = "";

int N = 2;
int Volume = 50;
String nameList[100];


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

    Serial.begin(115200);

    while (!Serial) {
        ;
    }
    
    if (!SD.begin()) {
        while (1);
    }
    
    audio.speakerPin = audio_output;
    SD.begin(SD_CSPin);
    audio.CSPin = SD_CSPin;

    myFile = SD.open("index.txt");
    if (myFile) {
        String _txt = "";
        while (myFile.available()) {
            _txt += (char)myFile.read();
        }
        myFile.close();
        file_number = _txt.toInt();
    }
    else{
    }
}

void DisplayMenu::Start(){
    
    /* lcd.init();              // for I2C
    lcd.backlight(); */
    
    lcd.begin(16, 2);           // for simulation

    lcd.clear();

    MenuPosition1(1,"Record Voice", "Recordings");
    delay(500);
}

void DisplayMenu::startRecording() {

    itoa(file_number, fileSlNum, 10);
    strcat(file_name, filePrefixname);
    strcat(file_name, fileSlNum);
    strcat(file_name, exten);
    delay(500);
    audio.startRecording(file_name, 8000, input_signal);
    //delay(10000);
    }


void DisplayMenu::stopRecording() {
    audio.stopRecording(file_name);
    int i;
    for (i = 0; i < 50; i++) {
        file_name[i] = 0x00;
    }
    SD.remove("index.txt");
    file_number++;
    myFile = SD.open("index.txt", FILE_WRITE);
    if (myFile) {
        myFile.print(String(file_number));
    }
    myFile.close();
    
}


void DisplayMenu::startPlayback(String fileName) {
    fileName += ".WAV";
    if (SD.exists(fileName)) {
        char play_filename[fileName.length()];
        fileName.toCharArray(play_filename, fileName.length());
        audio.setVolume(Volume);
        audio.play(play_filename);
        //audio.stopPlayback();
    }
    else {
        //Serial.println(fileName + " That file doesn't exist");
    }
    
}

void DisplayMenu::deleteRecording(String fileName) {
    fileName += ".WAV";
    if (SD.exists(fileName)) {
        SD.remove(fileName);
        if (SD.exists(fileName)) {
            //Serial.println("\nFile removing failed");
        }
        else {
            //Serial.println("\nFile removing successed");
        }
    }
    else {
        //Serial.println("\nFile isn't there");
    }
}


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



void DisplayMenu::Enter(){
    if (_Position==1){
        MenuPosition1(4, "Start Recording", "");
    }
    else if (_Position==2){
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
        MenuPosition1(6, "Play Recording", "Change Volume");
    }
    else if (_Position==4){
        MenuPosition2(5, "Stop Recording", "");
        lcd.setCursor(2, 0);
        lcd.print("Recording...");
        startRecording();
        
    }
    else if (_Position==5){
        lcd.clear();
        lcd.print("Saving File");
        stopRecording();
        delay(1000);
        lcd.clear();
        lcd.print("File Saved");
        delay(1000);
        MenuPosition1(1, "Record Voice", "Recordings");
    }
    else if (_Position==11){
        deleteRecording(nameList[_FileIndex]);
        lcd.clear();
        lcd.print("File Deleted");
        _FileIndex = 0;
        for (int i = 0; i<100; i++){
            nameList[i] = "";
        }
        delay(1000);
        MenuPosition1(1, "Record Voice", "Recordings");
    }
    else if (_Position==6){
        MenuPosition2(14, "Stop", "");
        lcd.setCursor(3, 0);
        lcd.print("Playing...");
       
        startPlayback(nameList[_FileIndex]);
    }
    /* else if (_Position==14){
        audio.disable();
        //audio.stopPlayback();
       //audio.pause();
       MenuPosition2(16, "Play", "Stop");
       
       
    }
    else if (_Position==16){
        audio.disable();
        MenuPosition2(14, "Pause", "Stop");
        
     
    } */
    else if (_Position==14){
        audio.disable();
        MenuPosition1(6, "Play Recording", "Change Volume");
    }
    else if (_Position==7){
        //MenuPosition1(18, "Increase Volume", "Decrease Volume");
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Volume");
        lcd.setCursor(7,1);
        lcd.print(Volume);
        _Position = 18;
        delay(250);
    }
    else if (_Position==8){
        MenuPosition1(12, "High Pitch", "Low Pitch");
    }
    else if (_Position == 18){
        MenuPosition1(7, "Change Volume", "Change Pitch");
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
   /*  else if (_Position==14){
        MenuPosition2(14, "Pause", "Stop");
    }
    else if (_Position==15){
        MenuPosition2(14, "Pause", "Stop");
    }
    else if (_Position==16){
        MenuPosition2(16, "Play", "Stop");
    }
    else if (_Position==17){
        MenuPosition2(16, "Play", "Stop");
    } */
    else if (_Position==18){
        Volume++;
        //Increase volume in tmrpcm
        /* lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Volume:");
        lcd.setCursor(9,0);
        lcd.print(Volume);
        delay(1000);
        MenuPosition1(18, "Increase Volume", "Decrease Volume"); */
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Volume");
        lcd.setCursor(7,1);
        lcd.print(Volume);
        _Position = 18;
        delay(250);
    }
    else if (_Position==19){
        _FileIndex = _FileIndex;
        MenuPosition1(19, nameList[_FileIndex], nameList[_FileIndex + 1]);
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
   /*  else if (_Position==14){
        MenuPosition2(15, "Stop", "");
    }
    else if (_Position==15){
        MenuPosition2(15, "Stop", "");
    }
    else if (_Position==16){
        MenuPosition2(17, "Stop","");
    }
    else if (_Position==17){
        MenuPosition2(17, "Stop","");
    } */
    else if (_Position==18){
        Volume--;
        //decrease volume in tmrpcm
        /* lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Volume:");
        lcd.setCursor(9,0);
        lcd.print(Volume);
        delay(1000);
        MenuPosition1(18, "Increase Volume", "Decrease Volume"); */
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("Volume");
        lcd.setCursor(7,1);
        lcd.print(Volume);
        _Position = 18;
        delay(250);
    }
    else if (_Position>=19){
        _FileIndex++;
        _Position++;
       
        if (_FileIndex >=  N){
                --_FileIndex;
                --_Position;
                MenuPosition1(_Position, nameList[_FileIndex], "");
        }
        /* else if (_FileIndex == N){
            MenuPosition1(_Position, nameList[_FileIndex], "");
        } */
        else{
        MenuPosition1(_Position, nameList[_FileIndex], nameList[_FileIndex + 1]);
        }
    }
 }



void DisplayMenu::Back(){   
  if (_Position == 1 or _Position ==2 or _Position == 3) {           
        MenuPosition1(1, "Record Voice", "Recordings");
  }
  else if (_Position == 4 or _Position == 5){
        MenuPosition1(1, "Record Voice", "Recordings");
  }
  else if (_Position>=19){
        MenuPosition1(2, "Recordings", "Factory Reset");
  }
  else if (_Position==17){
        MenuPosition1(6, "Play Recording", "Change Volume");
  }
  else if (_Position == 6 or _Position == 7 or _Position == 8 or _Position == 9 or _Position == 10 or _Position == 11){
        MenuPosition1(19, nameList[_FileIndex], nameList[_FileIndex + 1]);
  } 
  else if (_Position == 12 or _Position == 13){
        MenuPosition1(8, "Change Pitch", "Include Echo");
  }
  else if (_Position == 18){
        MenuPosition1(7, "Change Volume", "Change Pitch");
  }
}