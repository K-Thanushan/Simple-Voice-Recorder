/*
* Date   : 30-May-2021

* project: Simple Voice Recorder
*
* Authors: 190592X - Nisitha Maneesha
*          190595J - Maleesha Siriwardana
*          190610E - Gevindu Ganganath
*          190621M - Thanushan k.  
*
* This code uses a text file to store the audio samples
* Modifications are done based on that data
*/

#include "Arduino.h"
#include "VoiceRecorder.h"
#include <SPI.h>
#include <SD.h>
#include <MCP4725.h>
#include <Wire.h>
#include <Complex.h>


/// I2C enabling for physical implementation 

 /* 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);   */          /// also do changes in 'start' function
                                           
 /// For simulation
 
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 3, 4, 5, 6, 7);    
 
#define SD_CSPin 53

MCP4725 dac(SDA, SCL);

File myFile, root;

char filePrefixname[50] = "aud";
char exten[10] = ".txt";
char file_name[50];
int file_number = 0;
volatile char fileSlNum[20] = "";

int MicPin = A0; 
int SpeakerPin = 11;
int Fs = 8000;

int N ;
int Volume = 5;
String nameList[100];

volatile bool state = false;
volatile bool isRecording = false;
volatile bool isPlaying = false;

String VoiceRecorder::threedig(int decimal){                
  String  str_num = String(decimal);
  while (str_num.length()<3){
    str_num = "0"+str_num;
  }
  return str_num;
}

static void VoiceRecorder::changeState(){
    if (isRecording or isPlaying){
        state = !state;
    }
}


VoiceRecorder::VoiceRecorder(int UpButton, int DownButton, int EnterButton, int BackButton, int Position, int FileIndex)
{
    pinMode(EnterButton, INPUT); 
    pinMode(BackButton, INPUT);
    pinMode(DownButton, INPUT);
    pinMode(UpButton, INPUT); 
    attachInterrupt(digitalPinToInterrupt(EnterButton), changeState, RISING);

    int _enter = EnterButton;
    int _back = BackButton;
    int _down = DownButton;
    int _up = UpButton;
    int _Position = Position;  
    int _FileIndex = FileIndex;  

    dac.begin();
    
    if (!SD.begin(SD_CSPin)) {
        while (1);
    }
    

    myFile = SD.open("index.txt");
    if (myFile) {
        String _txt = "";
        while (myFile.available()) {
            _txt += (char)myFile.read();
        }
        myFile.close();
        file_number = _txt.toInt();
    }
}

void VoiceRecorder::Start(){
    
    /* lcd.init();              // for I2C
    lcd.backlight(); */
    
    lcd.begin(16, 2);           // for simulation

    lcd.clear();
    lcd.setCursor(4, 0);
    lcd.print("Welcome!");
    delay(1000);
    MenuPosition1(1,"Record Voice", "Recordings");
    
}

void VoiceRecorder::startRecording() {

    itoa(file_number, fileSlNum, 10);
    strcat(file_name, filePrefixname);
    strcat(file_name, fileSlNum);
    strcat(file_name, exten);
    isRecording = !isRecording;

    File myFile = SD.open(file_name,FILE_WRITE); 
    while (1){
      int audio_in_analog = analogRead(MicPin)/4;
      String three_bits = threedig(audio_in_analog);
      myFile.print(three_bits);
      delay(1/Fs);
      if (state){
          break;
      }
    }
    isRecording = !isRecording;
    state = !state;
    myFile.close();
    lcd.clear();
    lcd.print("Saving audio");
    stopRecording();
    delay(750);
    lcd.clear();
    lcd.print("Saved to");
    lcd.setCursor(9, 0);
    lcd.print("AUD"+String(file_number-1));
    delay(750);
    MenuPosition1(1, "Record Voice", "Recordings");
}


void VoiceRecorder::stopRecording() {
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


void VoiceRecorder::startPlayback(String fileName) {

    isPlaying = !isPlaying;

    fileName += ".TXT";
    File outfile = SD.open(fileName, FILE_READ);

    String _txt = "";
    int count = 0 ;

    while (outfile.available()){

      if (count<3){    
        _txt += (char) outfile.read();
        count++;
      }
      else{
        int alg_val = _txt.toInt();
        _txt = "";
        count = 0 ;
        alg_val = 2048+8*alg_val;
        dac.setValue(alg_val);
      }

      if (state){
          state = !state;
          break;
      }
    }
    isPlaying = !isPlaying;
    outfile.close();
    delay(500);
    MenuPosition1(6, "Play Recording", "Change Volume");
} 

void VoiceRecorder::deleteRecording(String fileName) {
    fileName += ".txt";
    if (SD.exists(fileName)) {
        SD.remove(fileName);
    }
}

void VoiceRecorder::reset(){
    root = SD.open("/");
  
        while (true) {
            File entry =  root.openNextFile();
            if (! entry) {
            break;
            }
    
            SD.remove(entry.name());
            entry.close();
        }
    root.close();
    Volume = 5;
}

void VoiceRecorder::freq_shift(String fileName,int shiftFrq){
  Serial.begin(9600);
  Serial.println("welcome");
  isPlaying = !isPlaying;

  fileName += ".TXT";
  File freq_file = SD.open(fileName, FILE_READ);
  
  String _txt = "";
  int count = 0 ;
  float time_val = 0;
  
  while (freq_file.available()){
      if (count<3){    
        _txt += (char) freq_file.read();
        count++;
      }
      else{
        int alg_val = _txt.toInt();
        Complex angle(cos(shiftFrq*1/Fs*time_val),sin(shiftFrq*1/Fs*time_val));
        int shift_val = alg_val*(angle.real()+angle.imag());
        dac.setValue(shift_val);
        Serial.println(shift_val);
        _txt = "";
        count = 0 ;
        time_val += 1/8000;
      }
      if (state){
          state = !state;
          break;
      }
   }
   isPlaying = !isPlaying;
   freq_file.close();
   delay(500);
   MenuPosition1(12, "High Pitch", "Low Pitch");
}


void VoiceRecorder::MenuPosition1(int a, String Displaytext1, String Displaytext2){
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

void VoiceRecorder::MenuPosition2(int a, String Displaytext3, String Displaytext4){
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



void VoiceRecorder::Enter(){
    if (_Position==1){
        MenuPosition1(4, "Start Recording", "");
    }

    else if (_Position==2){
        int count = 0;
        root = SD.open("/");
  
        while (true) {
            File entry =  root.openNextFile();
            if (! entry) {
            break;
            }
    
            String name = entry.name();
            int len = name.length();
  
            if (name.substring(len-3, len) == "TXT" and name != "INDEX.TXT" ){
            nameList[count] = name.substring(0, len-4);
            count ++;
            }
            entry.close();
        }
        root.close();
        N = count;
        if (N == 0){
            lcd.clear();
            lcd.print("SD is empty");
        }
        else{
        MenuPosition1(19, nameList[_FileIndex], nameList[_FileIndex + 1]);
        }
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

    else if (_Position==11){
        deleteRecording(nameList[_FileIndex]);
        lcd.clear();
        lcd.print(nameList[_FileIndex]+" deleted");
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
        delay(1000);
        startPlayback(nameList[_FileIndex]);
    }

    /* else if (_Position==14){
        MenuPosition1(6, "Play Recording", "Change Volume");
    } */

    else if (_Position==7){
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
    else if (_Position == 3){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Confirm factory ");
        lcd.setCursor(0, 1);
        lcd.print("reset ?");
        _Position = -1;
        delay(750);
    }
    else if (_Position == -1){
        reset();
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Reseting Success");
        delay(750);
        MenuPosition1(1, "Record Voice", "Recordings");
    }
    else if (_Position == 12){
        MenuPosition2(14, "Stop", "");
        lcd.setCursor(3, 0);
        lcd.print("Playing...");
        delay(1000);
        freq_shift(nameList[_FileIndex], -1500);
    }
    else if (_Position == 13){
        MenuPosition2(14, "Stop", "");
        lcd.setCursor(3, 0);
        lcd.print("Playing...");
        delay(1000);
        freq_shift(nameList[_FileIndex], 1500);
    }
}

void VoiceRecorder::Up(){
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

    else if (_Position==18){
        if (Volume != 7){
        Volume++;
        }
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

void VoiceRecorder::Down(){
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

    else if (_Position==18){
        if (Volume != 0){
        Volume--;
        }
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
        else{
        MenuPosition1(_Position, nameList[_FileIndex], nameList[_FileIndex + 1]);
        }
    }
 }



void VoiceRecorder::Back(){   
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
  else if (_Position == -1){
       MenuPosition1(1, "Record Voice", "Recordings");
  }
}
