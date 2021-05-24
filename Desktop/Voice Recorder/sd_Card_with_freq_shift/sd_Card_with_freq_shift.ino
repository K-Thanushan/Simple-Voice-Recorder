#include <SPI.h>
#include <SD.h>
#include <Complex.h>
#include <TMRpcm.h>
#define SD_CSPin 4

TMRpcm audio;
File myFile,uFile,root;

int file_number = 0;
char filePrefixname[10] = "aud";
char exten[10] = ".wav";
char exten_txt[10] = ".txt";

int mic_pin = A0;
int player = 3;
int take_input = 5;
int file_write = 6;
int del_file = 7;
int give_output = 8;
int audio_output =9;
int freq_shiftpin = 10;
int Fs = 44100;
int sample_rate = 16000;

String threedig(int decimal){
  String  str_num = String(decimal);
  while (str_num.length()<3){
    str_num = "0"+str_num;
  }
  return str_num;
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

void freq_shift(String file_name,int shift,int Fs){
  if (SD.exists(file_name)){
    myFile = SD.open(file_name, FILE_WRITE);              // open the text file which contains the bit stream
    Serial.println(file_name+" opened");
    if (myFile){
        String _txt="";         //extract data from it
        while (myFile.available()) {
          _txt += (char)myFile.read(); 
        }
        myFile.close();
        int lengthoftxt = _txt.length();
        char *out_file = "fm.wav";
        audio.createWavTemplate(out_file,16000);
        int t = 0;
        uFile = SD.open(out_file, FILE_WRITE);
        for(int y =0; y<lengthoftxt; y=y+3){
          String nw = _txt.substring(y,y+3);        // gathering 3bits to 1 group
          int dec = nw.toInt();                    // convert string val to decimal
          Complex angle(cos(shift*1/Fs*y),sin(shift*1/Fs*y));
          float dec_4 = dec*4*(angle.real()+angle.imag());//*angle;    //multiply decimal val with e^(jwt)
          uFile.write(dec_4);
        }
        uFile.close();
        audio.finalizeWavTemplate(out_file);
        audio.volume(5);
        audio.play(out_file);
        audio.disable();
        if (SD.exists(out_file)){
          SD.remove(out_file);
        }
    }
  }
}

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
  pinMode(mic_pin,INPUT);
  pinMode(player,INPUT);
  pinMode(take_input,INPUT);
  pinMode(file_write,INPUT);
  pinMode(give_output,INPUT);
  pinMode(del_file,INPUT);
  pinMode(freq_shiftpin,INPUT);
  audio.CSPin = SD_CSPin;
}

void loop() {
  if (digitalRead(take_input)== LOW){
    char fileSlNum[20] = "";
    itoa(file_number, fileSlNum, 10);
    char file_name[40] = "";
    char file_name_txt[40] = "";
    strcat(file_name, filePrefixname);
    strcat(file_name, fileSlNum);
    strcat(file_name, exten);
    strcat(file_name_txt, filePrefixname);
    strcat(file_name_txt, fileSlNum);
    strcat(file_name_txt, exten_txt);
     
    audio.startRecording(file_name, sample_rate, mic_pin);    // tmrpcm recording started
    myFile = SD.open(file_name_txt,FILE_WRITE);                // text file is created
    
    while (digitalRead(file_write) == HIGH){                    // until stop btn click, myFile is updating & tmrpcm is recording
        int audio_in_analog = analogRead(A0)/4;
        myFile.print(threedig(audio_in_analog));
        delay(1/Fs);
    }
    
    myFile.close();
    audio.stopRecording(file_name);
    
    file_number++;
  }
  
if (digitalRead(del_file) == HIGH) {                          // file deleting
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

  if (digitalRead(give_output)==HIGH){                            // play audio
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
  
  if (digitalRead(freq_shiftpin)==HIGH){                       // to have freq shifting
    Serial.print("\nEnter the file name to delete: \n ");
    String shift_file = Serial.readStringUntil('\n');
    freq_shift("aud0.txt",25,Fs);
    if (SD.exists(shift_file)){
        freq_shift(shift_file,25,Fs);
      }else{
        Serial.println("\nFile isn't there");
      }
  }
}
