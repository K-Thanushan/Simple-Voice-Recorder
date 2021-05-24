

#include <SD.h>               // need to include the SD library
#define SD_ChipSelectPin 53 
#include <TMRpcm.h>           //  Lib to play wav file
#include <SPI.h>

TMRpcm tmrpcm;
void setup() {
tmrpcm.speakerPin = 11; 
  Serial.begin(9600);
if (!SD.begin(SD_ChipSelectPin)) {  // see if the card is present and can be initialized:
    Serial.println("SD fail");  
    return;   // don't do anything more if not
  }
  else{   
    Serial.println("SD ok");   
  }
  tmrpcm.play("1.wav");
  tmrpcm.volume(5); 
}

void loop() {
  // put your main code here, to run repeatedly:

}
