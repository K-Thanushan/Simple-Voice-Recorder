#include <SPI.h>
#include <SD.h>

File myFile,root;

int input_signal = A0; // Audio input 6 - A0
int take_input = 5;
int del_file = 6;
int file_write = 7;
int give_output = 8;
int led_output = 9;
int Fs = 10;
String audio_in_binary;

String dec2bin(int decimal){
  String binary;
  while (decimal) {
    binary = String(decimal % 2) + binary;
    decimal /= 2;
  }
  while (binary.length()<8) {
    binary = "0"+binary;
  }
  return binary;
}
void printDirectory(File dir, int numTabs) {
      while (true) {
     
        File entry =  dir.openNextFile();
        if (! entry) {
          // no more files
          break;
        }
        Serial.println("Current available files");
        for (uint8_t i = 0; i < numTabs; i++) {
          Serial.print('\t');
        }
        Serial.println(entry.name());
        if (entry.isDirectory()) {
          Serial.println("/");
          printDirectory(entry, numTabs + 1);
        } else {
          // files have sizes, directories do not
          Serial.print("\t\t");
          //Serial.println(entry.size(), DEC);
        }
        entry.close();
      }
}
int bin2dec(String binary){
  int decimal = 0;
  /*while (*cha){
  }
  return decimal;*/
  for (int po = 0 ; po<binary.length(); po++){
    decimal <<=1;
    if (binary[po]++ == '1') decimal  |= 1;
  }
  return decimal;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only
    }
    Serial.println("Initializing SD card...");
    if (!SD.begin()) {
      Serial.println("initialization failed!");
      while (1);
    }
    Serial.println("initialization done.");
    pinMode(take_input, INPUT);
    pinMode(input_signal,INPUT);
    pinMode(give_output,INPUT);
    pinMode(led_output,OUTPUT);
}

void loop(){
  if (digitalRead(take_input)== LOW){
    int audio_in_analog = analogRead(A0)/4;
    audio_in_binary += dec2bin(audio_in_analog);
    Serial.println("reading the audio ");
    delay(Fs);
  }

  if (digitalRead(file_write)==HIGH){
    root = SD.open("/");
    printDirectory(root, 0);
    Serial.println("\nEnter the file name to save: ");
    String save_file = Serial.readStringUntil('\n');
    if (save_file != ""){
      myFile = SD.open(save_file, FILE_WRITE);
      if (myFile) {
        Serial.println("\n\nWriting to "+save_file);
        myFile.print(audio_in_binary);
        myFile.close();
        Serial.println("done.");
      } else {
        Serial.println("\nerror opening "+save_file);
      }
      delay(3000);
    }
  }

  if (digitalRead(del_file)==HIGH){
      root = SD.open("/");
      printDirectory(root, 0);
      Serial.print("\nEnter the file name to delete: \n ");
      String read_file = Serial.readStringUntil('\n');
      if (SD.exists(read_file)){
        SD.remove(read_file);
         if (SD.exists(read_file)){
            Serial.println("\nFile removing failed");
          }else{
            Serial.println("\nFile removing successed");
          }
      }else{
        Serial.println("\nFile isn't there");
      }
    
  root = SD.open("/");
  printDirectory(root, 0);
  delay(3000);
  }

  if (digitalRead(give_output)==HIGH){
    root = SD.open("/");
    printDirectory(root, 0);
    Serial.println("Enter the file name read: \n");
    String read_file = Serial.readStringUntil('\n');
    if (SD.exists(read_file)){
      myFile = SD.open(read_file);
      Serial.println("\n"+read_file+" is opened");
      if (myFile) {
        String _txt="";
        while (myFile.available()) {
          _txt += (char)myFile.read(); 
        }
        myFile.close();
          int lengthoftxt = _txt.length();
          for(int y =0; y<lengthoftxt; y=y+8){
            String nw = _txt.substring(y,y+8);
            int dec = bin2dec(nw);
            int dec_4 = dec*4;
            //Serial.print(" decimal ");
            Serial.println(dec_4);
            digitalWrite(led_output,dec_4);
            delay(500);
          }
        digitalWrite(led_output,LOW);
        Serial.println("Blinking is over");
      }else{
        Serial.println("error opening "+read_file);
      }
    }else{
      Serial.println(read_file+" That file doesn't exist");
    }
    delay(3000);
  }
}
