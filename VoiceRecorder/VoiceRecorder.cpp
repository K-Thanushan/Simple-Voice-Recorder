/*
* Date   : 10-Aug-2021
*
* project: Simple Voice Recorder
*
* Authors: 190592X - Nisitha Maneesha
*          190595J - Maleesha Siriwardana
*          190610E - Gevindu Ganganath
*          190621M - Thanushan Kamalakkannan  
*
*/

#include "LCD.h"
#include "VoiceRecorder.h"
#include "arduino.h"
#include <SPI.h>
#include <SD.h>
#include <EEPROM.h>
#include <Complex.h>

#define SD_CSPin 10

unsigned short int Position = 0;
unsigned short int tempPosition = 0;
unsigned short int Volume = 5;
unsigned short int FileIndex = 0;
unsigned short int file_number = 0;
unsigned short int count = 0;
unsigned short int audio_in_analog;
const int delayTime = 125;
float startTime;
float endTime;

String FileList[15];
String sortingList[15];
const String pre = "AUD";
const String exten = ".TXT";

volatile bool isPlaying = false;
volatile bool isRecording = false;
volatile bool state = false;

File aud_file, root;

LCD MyLCD(1);


static void VoiceRecorder::changeState(){
    if (isRecording || isPlaying){
		delay(50);
		state = true;
	}
}

void VoiceRecorder::ADC_init(){
	//set AREF to VCC
	ADMUX = (1 << REFS0);
	//enable ADC and set pre-scaler to 128
	ADCSRA = (1 << ADEN)  | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2); 
}

int VoiceRecorder::readADC(int channel){
	//enable the ADC channel
	channel &= 0b00000111;
	ADMUX |= channel;
	//start conversion
	ADCSRA |= (1 << ADSC);
	//wait till conversion is complete
	while (ADCSRA & (1 << ADSC));
	return (ADC);
}

VoiceRecorder::VoiceRecorder(int UpButton, int DownButton, int EnterButton, int BackButton){

	pinMode(UpButton, INPUT);
	pinMode(DownButton, INPUT);
	pinMode(EnterButton, INPUT);
	pinMode(BackButton, INPUT);
	attachInterrupt(digitalPinToInterrupt(EnterButton), changeState, RISING);

	if (!SD.begin(SD_CSPin)) {
    	while (1);
	}
    
	file_number = EEPROM.read(0);
	if (file_number == 255){
		file_number = 0;
	}
}

void VoiceRecorder::MenuPosition1(int a, String Displaytext1, String Displaytext2){

	const short int text1len = Displaytext1.length()+1;
	char text1[text1len];
	Displaytext1.toCharArray(text1, text1len);

	const short int text2len = Displaytext2.length()+1;
	char text2[text2len];
	Displaytext2.toCharArray(text2, text2len);
	
	Position = a;
	MyLCD.Clear();
	MyLCD.PrintString_xy(0, 0, ">");
	MyLCD.PrintString_xy(0, 1, text1);
	MyLCD.PrintString_xy(1, 0, text2);
	_delay_ms(250);
}

void VoiceRecorder::MenuPosition2(int a, int b, String Displaytext1, String Displaytext2){

	const short int text1len = Displaytext1.length()+1;
	char text1[text1len];
	Displaytext1.toCharArray(text1, text1len);

	const short int text2len = Displaytext2.length()+1;
	char text2[text2len];
	Displaytext2.toCharArray(text2, text2len);
	
	Position = a;
	MyLCD.Clear();
	MyLCD.PrintString_xy(1, 0, ">");
	MyLCD.PrintString_xy(0, b, text1);
	MyLCD.PrintString_xy(1, 1, text2);
	_delay_ms(250);
}

String VoiceRecorder::threedig(int decimal){ 
	String  str_num;               
  	str_num = String(decimal);
  	while (str_num.length()<3){
    	str_num = "0"+str_num;
  	}
  	return str_num;
}

void VoiceRecorder::startRecording(){
	String fileName;
	isRecording = true;

	if (file_number <= 9){
		fileName = pre+"0"+String(file_number)+exten;
	}
	else{
		fileName = pre+String(file_number)+exten;
	}
	aud_file = SD.open(fileName, FILE_WRITE);

	while (1){
		startTime = micros();
		audio_in_analog = readADC(0)/4;
		String three_bits = threedig(audio_in_analog);
		aud_file.print(three_bits);
		if (state){
			break;
		}
		endTime = micros();
		if ((endTime-startTime) < delayTime){
			delayMicroseconds(delayTime-(endTime-startTime));
		}
	}

	aud_file.close();
	state = false;
	isRecording = false;
	MyLCD.Clear();
	MyLCD.PrintString("Audio saved to");
	MyLCD.PrintString_xy(1, 0, "AUD");
	char tempNum[3];
	if (file_number>9){
		itoa(file_number, tempNum, 10);
		MyLCD.PrintString_xy(1, 3, tempNum);
	}
	else{
		char zero[2] = "0";
		itoa(file_number, tempNum, 10);
		strcat(zero, tempNum);
		MyLCD.PrintString_xy(1, 3, zero);
	}
	delay(1000);
	file_number++;
	EEPROM.write(0, file_number);
	MenuPosition1(1, "Record Voice", "Recordings");
}

void VoiceRecorder::startPlayback(String fileName) {
	isPlaying = true;
	fileName += ".TXT";
	aud_file = SD.open(fileName, FILE_READ);

	String _txt = "";
	int bits = 0 ;

	TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);   	// OCR2B compare match, fast PWM init
	TCCR2B = _BV(CS21);									// 8 bit pre scalar
	OCR2B = 00;									

	while (aud_file.available()){

		startTime =micros();
		if (bits<3){
			_txt += (char) aud_file.read();
			bits++;
		}
		else{
			int alg_val = _txt.toInt();
			_txt = "";
			bits = 0;
			OCR2B = alg_val;
		}

		if (state){
			state = false;
			break;
		}
		endTime = micros();
		if ((endTime-startTime) < delayTime){
			delayMicroseconds(delayTime-(endTime-startTime));
		}
	}
	OCR2B = 00;
	TCCR2B = 0;
	isPlaying = false;
	aud_file.close();
	delay(50);
	MenuPosition1(6, "Play Recording", "Sound Effect");
}

void VoiceRecorder::freq_shift(String fileName){

  isPlaying = true;

  fileName += ".TXT";
  aud_file = SD.open(fileName, FILE_READ);
  
  String _txt = "";
  int bits = 0 ;
  float time_val = 0;

  TCCR2A = _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
  TCCR2B = _BV(CS21);
  OCR2B = 00;
  
  while (aud_file.available()){
      if (bits<3){    
        _txt += (char) aud_file.read();
        bits++;
      }
      else{
        int alg_val = _txt.toInt();
        Complex angle(cos(-2500*1/8000*time_val),sin(-2500*1/8000*time_val));
        int shift_val = alg_val*(angle.real()+angle.imag());
		OCR2B = shift_val;
        _txt = "";
        bits = 0 ;
        time_val += 1/8000;
      }
      if (state){
        state = false;
        break;
      }
   }
   OCR2B = 00;
   TCCR2B = 0;
   isPlaying = false;
   aud_file.close();
   delay(50);
   MenuPosition1(7, "Sound Effect", "DeleteRecording");
}

void VoiceRecorder::Start(void)
{
	MyLCD.Initialize();
	ADC_init();
	MyLCD.PrintString_xy(0, 4, "Welcome!");
	_delay_ms(1000);
	MyLCD.Clear();
	MenuPosition1(1, "Record Voice", "Recordings");
}

void VoiceRecorder::Enter(void)
{
	if (Position==1){
		MenuPosition1(4, "Start Recording", "");
	}
	else if (Position==2){

		for (int i=0; i<15; i++){
			FileList[i] = "";
		}

		FileIndex = 0;
		count = 0;
		root = SD.open("/");

		while (true) {
			String name;
    		aud_file =  root.openNextFile();
    		if (!aud_file) {
        		break;
    		}
    		name = aud_file.name();
    		int len = name.length();
			if (name.substring(len-3, len) == "TXT"){
    			sortingList[count] = name.substring(0, len-4);
				count ++;
			}
    		aud_file.close();
		}
		root.close();

		String currMax;
		for (int j=0; j<count; j++){
			currMax = sortingList[0];
			unsigned short int poss=0;
			for (int i=0; i<count; i++){
				if (currMax<sortingList[i]){
					currMax = sortingList[i];
					poss = i;
				}
			}
		sortingList[poss] = "0";
		FileList[j] = currMax;
		}

		for (int i=0; i<15; i++){
			sortingList[i] = "";
		}

		if (count == 0){
			MyLCD.Clear();
			MyLCD.PrintString_xy(0, 4,"No Files");
			Position = 17;
		}
		else{
			MenuPosition1(20, FileList[FileIndex], FileList[FileIndex + 1]);
		}
	}

	else if (Position == 3){
		MyLCD.Clear();
		MyLCD.PrintString_xy(0, 0, "Confirm cleanup?");
		Position = 16;
		_delay_ms(750);
	}
    else if (Position==4){
		MenuPosition2(5, 2,"Recording...", "Stop Recording");
        startRecording();
		
    }
    else if (Position==8){
		String file = FileList[FileIndex]+".TXT";
	
		if (SD.exists(file)){
			SD.remove(file);
		}
		MyLCD.Clear();
		MyLCD.PrintString_xy(0, 4, "Deleted");
		FileIndex = 0;
		count = 0;
		delay(1000);
		for (int i=0; i<15; i++){
			FileList[i] = "";
		}
        MenuPosition1(2, "Recordings", "Cleanup SD");
    }

    else if (Position==6){
		MenuPosition2(14, 3, "Playing...", "Stop");
        startPlayback(FileList[FileIndex]);
    }
    else if (Position==7){
		MenuPosition2(14, 3, "Playing...", "Stop");
		freq_shift(FileList[FileIndex]);
    }
	else if (Position == 16){
		file_number = 0;
		EEPROM.write(0, file_number);
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

		MyLCD.Clear();
		MyLCD.PrintString_xy(0, 0, "Cleanup Success");
		_delay_ms(1000);
		MenuPosition1(1, "Record Voice", "Recordings");
	}
	else if (Position>=20){
		tempPosition = Position;
		MenuPosition1(6, "Play Recording", "Sound Effect");
	}
}

void VoiceRecorder::Up(void)
{
	if (Position == 1)
	{
		MenuPosition1(3, "Cleanup SD", "Record Voice");
	}
	else if (Position == 2)
	{
		MenuPosition1(1, "Record Voice", "Recordings");
	}
	else if (Position == 3)
	{
		MenuPosition1(2, "Recordings", "Cleanup SD");
	}
	else if (Position == 4)
	{
		MenuPosition1(4, "Start Recording", "");
	}
	else if (Position == 7)
	{
		MenuPosition1(6, "Play Recording", "Sound Effect");
	}
	else if (Position == 8)
	{
		MenuPosition1(7, "Sound Effect", "DeleteRecording");
	}
	else if (Position==20){
		MenuPosition1(20, FileList[FileIndex], FileList[FileIndex + 1]);
	}
	else if (Position>20){
		FileIndex --;
		Position--;
		MenuPosition1(Position, FileList[FileIndex], FileList[FileIndex + 1]);
	}
}

void VoiceRecorder::Down(void)
{
	if (Position == 1)
	{
		MenuPosition1(2, "Recordings", "Cleanup SD");
	}
	else if (Position == 2)
	{
		MenuPosition1(3, "Cleanup SD", "Record Voice");
	}
	else if (Position == 3)
	{
		MenuPosition1(1, "Record Voice", "Recordings");
	}
	else if (Position == 6)
	{
		MenuPosition1(7, "Sound Effect", "DeleteRecording");
	}
	else if (Position == 7)
	{
		MenuPosition1(8, "DeleteRecording", "");
	}
	else if (Position>=20){
		if (FileIndex >= count - 1){
			MenuPosition1(Position, FileList[FileIndex], "");
		}
		else{
			FileIndex++;
			Position++;
			MenuPosition1(Position, FileList[FileIndex], FileList[FileIndex + 1]);
		}
	}
}

void VoiceRecorder::Back(void)
{
	if (Position == 1 || Position ==2 || Position == 3) {
		MenuPosition1(1, "Record Voice", "Recordings");
	}
	else if (Position == 4 || Position == 5){
		MenuPosition1(1, "Record Voice", "Recordings");
	}
	else if (Position>=20 || Position==17){
		MenuPosition1(2, "Recordings", "Cleanup SD");
	}
	else if (Position == 6 || Position == 7 || Position == 8 || Position == 9 || Position == 10 || Position == 11){
		MenuPosition1(tempPosition, FileList[FileIndex], FileList[FileIndex + 1]); 
	}
	else if (Position == 16){
		MenuPosition1(3, "Cleanup SD", "Record Voice");
	}
}
