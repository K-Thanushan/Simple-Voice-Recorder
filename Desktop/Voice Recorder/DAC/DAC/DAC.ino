#include <SPI.h>
#include <SD.h>
#define SD_CSPin 53
#include <MCP4725.h>
#include <Complex.h>

MCP4725 dac(SDA, SCL);

int mic_pin = A0;
int take_input = 14;
int start_record = 15;
int give_output = 16;
int Fs = 8000;

String threedig(int decimal){                 // create three bit long analog vals
  String  str_num = String(decimal);
  while (str_num.length()<3){
    str_num = "0"+str_num;
  }
  return str_num;
}

//String dec2bin(int decimal){            //use in case of 8 bit sampling required
//  String binary;
//  while (decimal) {
//    binary = String(decimal % 2) + binary;
//    decimal /= 2;
//  }
//  while (binary.length()<8) {
//    binary = "0"+binary;
//  }
//  return binary;
//}
//
//int bin2dec(String binary){                   //use in case of 8 bit sampling required
//  int decimal = 0;
//  for (int po = 0 ; po<binary.length(); po++){
//    decimal <<=1;
//    if (binary[po]++ == '1') decimal  |= 1;
//  }
//  return decimal;
//}

void freq_shift(String file_name,int shift,int Fs){
  File freq_file = SD.open(file_name, FILE_WRITE);
  String _txt = "";
  int count = 0 ;
  int time_val = 0;
  while (freq_file.available()){
      if (count<3){     // to set 3 bits together
        _txt += (char) freq_file.read();
        count++;
      }else{
        int alg_val = _txt.toInt();
        Complex angle(cos(shift*1/Fs*time_val),sin(shift*1/Fs*time_val));
        float shift_val = alg_val*(angle.real()+angle.imag());
        dac.setValue(pgm_read_word(&(shift_val)));
        _txt = "";
        count = 0 ;
        time_val++;
      }
   }
}  

void setup() {
  // put your setup code here, to run once:
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
  pinMode(mic_pin ,INPUT);
  pinMode(take_input,INPUT);
  pinMode(start_record,INPUT);
  pinMode(give_output,INPUT);
  dac.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(take_input)== HIGH){
    delay(2000);
    File myFile = SD.open("out.txt",FILE_WRITE); 
    Serial.println("file open");
    String txt = "";
    while (digitalRead(start_record)==HIGH){
      int audio_in_analog = analogRead(mic_pin);
      //String val_bin = dec2bin(audio_in_analog);
      //myFile.print(val_bin);
      String three_bits = threedig(audio_in_analog);
      myFile.print(three_bits);
      Serial.println(three_bits);
      delay(1/Fs);
    }
    myFile.close();
    delay(2300);
  }
  if (digitalRead(give_output)==HIGH){
    File outfile = SD.open("out.txt", FILE_READ);
    Serial.println("out file open");
    String _txt = "";
    int count = 0 ;
    while (outfile.available()){
      if (count<3){     // to set 3 bits together
        _txt += (char) outfile.read();
        count++;
      }else{
        int alg_val = _txt.toInt();
        _txt = "";
        count = 0 ;
        Serial.println(alg_val);
        dac.setValue(pgm_read_word(&(alg_val)));
        //dac.setValue(alg_val);
      }
    }
  }
}
