/*
 * VoiceRecorder.cpp
 *
 * Created: 6/18/2021 7:11:45 PM
 *  Author: K.Thanushan
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include "LCD.h"
#include "VoiceRecorder.h"

int Position = 0;
int Volume = 5;
int FileIndex = 0;
int N = 5;
char FileList[][5] = {"AUD1", "AUD2", "AUD3", "AUD4", "AUD5"};

LCD MyLCD(1);

VoiceRecorder::VoiceRecorder(int d){

}
void VoiceRecorder::MenuPosition1(int a, char *Displaytext1, char *Displaytext2)
{
	Position = a;
	MyLCD.Clear();
	MyLCD.PrintString_xy(0, 0, ">");
	MyLCD.PrintString_xy(0, 1, Displaytext1);
	MyLCD.PrintString_xy(1, 0, Displaytext2);
	_delay_ms(250);
}

void VoiceRecorder::MenuPosition2(int a, int b, char *Displaytext1, char *Displaytext2)
{
	Position = a;
	MyLCD.Clear();
	MyLCD.PrintString_xy(1, 0, ">");
	MyLCD.PrintString_xy(0, b, Displaytext1);
	MyLCD.PrintString_xy(1, 1, Displaytext2);
	_delay_ms(250);
}

void VoiceRecorder::Start(void)
{
	MyLCD.Initialize();
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
        MenuPosition1(17, FileList[FileIndex], FileList[FileIndex + 1]);
    }
	else if (Position == 3){
		MyLCD.Clear();
		MyLCD.PrintString_xy(0, 0, "Confirm factory ");
		MyLCD.PrintString_xy(1, 0, "reset ?");
		Position = 16;
		_delay_ms(750);
	}
    else if (Position==4){
        //startRecording();
		MenuPosition2(5, 2,"Recording...", "Stop Recording");
    }
	else if (Position==5){
		//stopRecording();
		MenuPosition1(1, "Record Voice", "Recordings");
	}
    else if (Position==11){
        //deleteRecording(nameList[_FileIndex]);
        MyLCD.Clear();
        /*lcd.print(nameList[_FileIndex]+" deleted");
        _FileIndex = 0;
        for (int i = 0; i<100; i++){
            nameList[i] = "";
        }*/
        _delay_ms(1000);
        MenuPosition1(17, "", "");
    }

    else if (Position==6){
        //startPlayback(nameList[_FileIndex]);
		//_delay_ms(1000);
		MenuPosition2(14, 3, "Playing...", "Stop");
    }
    else if (Position==7){
        MyLCD.Clear();
		MyLCD.PrintString_xy(0, 5, "Volume");
		char List[4];
		itoa(Volume,List,10);
		MyLCD.PrintString_xy(1, 7, List);
		Position = 15;
		_delay_ms(500);
    }
    else if (Position == 8){
        MenuPosition1(12, "High Pitch", "Low Pitch");
    }
	else if (Position == 9){
		//echofunction();
		//_delay_ms(1000);
		MenuPosition2(14, 3, "Playing...", "Stop");
	}
	else if (Position == 10){
		//echofunction();
		//_delay_ms(1000);
		MenuPosition2(14, 3, "Playing...", "Stop");
	}
    else if (Position == 12){
        //freq_shift(nameList[_FileIndex], 1500);
		//_delay_ms(1000);
        MenuPosition2(14, 3, "Playing...", "Stop");
    }
    else if (Position == 13){
        //freq_shift(nameList[_FileIndex], 1500);
		//_delay_ms(1000);
		MenuPosition2(14, 3, "Playing...", "Stop");
    }
	else if (Position==14){
		//stopPlayback(nameList[_FileIndex]);
		//_delay_ms(1000);
		MenuPosition1(17, FileList[FileIndex], FileList[FileIndex + 1]);
	}
	else if (Position == 15){
		 MenuPosition1(7, "Change Volume", "Change Pitch");
	}
	else if (Position == 16){
		 //reset()
		 MyLCD.Clear();
		 MyLCD.PrintString_xy(0, 0, "Reseting Success");
		 _delay_ms(750);
		 MenuPosition1(1, "Record Voice", "Recordings");
	}
	else if (Position>=17){
		MenuPosition1(6, "Play Recording", "Change Volume");
	}
}

void VoiceRecorder::Up(void)
{
	if (Position == 1)
	{
		MenuPosition1(3, "Factory Reset", "Record Voice");
	}
	else if (Position == 2)
	{
		MenuPosition1(1, "Record Voice", "Recordings");
	}
	else if (Position == 3)
	{
		MenuPosition1(2, "Recordings", "Factory Reset");
	}
	else if (Position == 4)
	{
		MenuPosition1(4, "Start Recording", "");
	}
	else if (Position == 7)
	{
		MenuPosition1(6, "Play Recording", "Change Volume");
	}
	else if (Position == 8)
	{
		MenuPosition1(7, "Change Volume", "Change Pitch");
	}
	else if (Position == 9)
	{
		MenuPosition1(8, "Change Pitch", "Include Echo");
	}
	else if (Position == 10)
	{
		MenuPosition1(9, "Include Echo", "Fast Forward");
	}
	else if (Position == 11)
	{
		MenuPosition1(10, "Fast Forward", "DeleteRecording");
	}
	else if (Position == 13)
	{
		MenuPosition1(12, "High Pitch", "Low Pitch");
	}
	else if (Position == 15)
	{
		if (Volume != 7){
			Volume++;
		}
		MyLCD.Clear();
		MyLCD.PrintString_xy(0, 5, "Volume");
		char List[4];
		itoa(Volume,List,10);
		MyLCD.PrintString_xy(1, 7, List);
		Position = 15;
		_delay_ms(500);
	}
	else if (Position==17){
		FileIndex = FileIndex;
		MenuPosition1(17, FileList[FileIndex], FileList[FileIndex + 1]);
	}
	else if (Position>17){
		FileIndex --;
		Position--;
		MenuPosition1(Position, FileList[FileIndex], FileList[FileIndex + 1]);
	}
}

void VoiceRecorder::Down(void)
{
	if (Position == 1)
	{
		MenuPosition1(2, "Recordings", "Factory Reset");
	}
	else if (Position == 2)
	{
		MenuPosition1(3, "Factory Reset", "Record Voice");
	}
	else if (Position == 3)
	{
		MenuPosition1(1, "Record Voice", "Recordings");
	}
	else if (Position == 6)
	{
		MenuPosition1(7, "Change Volume", "Change Pitch");
	}
	else if (Position == 7)
	{
		MenuPosition1(8, "Change Pitch", "Include Echo");
	}
	else if (Position == 8)
	{
		MenuPosition1(9, "Include Echo", "Fast Forward");
	}
	else if (Position == 9)
	{
		MenuPosition1(10, "Fast Forward", "DeleteRecording");
	}
	else if (Position == 10)
	{
		MenuPosition1(11, "DeleteRecording", "");
	}
	else if (Position == 12)
	{
		MenuPosition1(13, "Low Pitch", "");
	}
	else if (Position == 15)
	{
		if (Volume != 7){
			Volume++;
		}
		MyLCD.Clear();
		MyLCD.PrintString_xy(0, 5, "Volume");
		char List[4];
		itoa(Volume,List,10);
		MyLCD.PrintString_xy(1, 7, List);
		Position = 15;
		_delay_ms(500);
	}
	else if (Position>=17){
		if (FileIndex >= N - 1){
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
	else if (Position==17){
		MenuPosition1(2, "Recordings", "Change Volume");
	}
	else if (Position == 6 || Position == 7 || Position == 8 || Position == 9 || Position == 10 || Position == 11){
		MenuPosition1(17, FileList[FileIndex], FileList[FileIndex + 1]);
	}
	else if (Position == 12 || Position == 13){
		MenuPosition1(8, "Change Pitch", "Include Echo");
	}
	else if (Position == 18){
		MenuPosition1(7, "Change Volume", "Change Pitch");
	}
	else if (Position == 16){
		MenuPosition1(3, "Factory Reset", "Record Voice");
	}
}