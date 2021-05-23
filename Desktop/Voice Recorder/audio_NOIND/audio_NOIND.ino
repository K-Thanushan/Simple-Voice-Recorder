#include <TMRpcm.h>
#include <SD.h>
#include <SPI.h>
TMRpcm audio;
int file_number = 0;
char filePrefixname[10] = "aud";
char exten[10] = ".wav";
const int mic_pin = A0;
const int sample_rate = 16000;
#define SD_CSPin 10
int player = 4;
int take_input = 5;
int del_file = 7;
int file_write = 6;
int give_output = 8;
int audio_output = 9;

void setup() {
  Serial.begin(9600);
   Serial.println("loading... SD card");
  if (!SD.begin(SD_CSPin)) {
    Serial.println("An Error has occurred while mounting SD");
  }
  Serial.println("initialization done.");
  while (!SD.begin(SD_CSPin)) {
    Serial.print(".");
    delay(500);
  }
  audio.speakerPin = audio_output;
  pinMode(mic_pin, INPUT);
  pinMode(player,INPUT);
  pinMode(take_input, INPUT);
  pinMode(file_write,INPUT);
  pinMode(give_output,INPUT);
  pinMode(del_file,INPUT);
  audio.CSPin = SD_CSPin;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(digitalRead(take_input)==HIGH){
      char fileSlNum[20] = "";
      itoa(file_number, fileSlNum, 10);
      char file_name[40] = "";
      strcat(file_name, filePrefixname);
      strcat(file_name, fileSlNum);
      strcat(file_name, exten);
      Serial.print("New File Name: ");
      Serial.println(file_name);
      audio.startRecording(file_name, sample_rate, mic_pin);
      Serial.println("startRecording ");
      while (digitalRead(file_write) == HIGH){
        Serial.println("Recording....");
        delay(3000);
      }
      audio.stopRecording(file_name);
      Serial.println("stopRecording");
      file_number++;
  }
  if (digitalRead(del_file) == HIGH) {
    Serial.print("\nEnter the file name to delete: \n ");
    String read_file = Serial.readStringUntil('\n');
    delay(3000);
    if (SD.exists(read_file)) {
      SD.remove(read_file);
      if (SD.exists(read_file)) {
        Serial.println("\nFile removing failed");
      } else {
        Serial.println("\nFile removing successed");
      }
    } else {
      Serial.println("\nFile isn't there");
    }
  }
  if (digitalRead(give_output)==HIGH){
    Serial.println("Enter the file name read: \n");
    String read_file = Serial.readStringUntil('\n');
    char read_filename[30] ;
    read_file.toCharArray(read_filename,read_file.length()) ;
    delay(2500);
    if (SD.exists(read_file)){
      audio.setVolume(5);
      Serial.println("playing started");
      audio.play(read_filename);
      if (digitalRead(player)==HIGH){
        audio.disable();
      }else{
        while(1){ 
        }
      }
    }
  }
}
