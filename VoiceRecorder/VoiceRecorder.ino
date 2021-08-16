/*
* Date   : 10-Aug-2021
*
* project: Simple Voice Recorder
*
* Authors: 190592X - Nisitha Maneesha
*          190595J - Maleesha Siriwardana
*          190610E - Gevindu Ganganath
*          190621M - Thanushan k.  
*
*/

#include "voiceRecorder.h" 

#define UpButton A1
#define DownButton A2
#define EnterButton 2
#define BackButton A3

VoiceRecorder voiceRecorder(UpButton, DownButton, EnterButton, BackButton);

void setup() {
    
    voiceRecorder.Start();
}

void loop() {
    int UpState = digitalRead(UpButton);
    if (UpState == HIGH){       
        voiceRecorder.Up();
    }

    int DownState = digitalRead(DownButton);
    if (DownState == HIGH){      
        voiceRecorder.Down();
    }

    int EnterState = digitalRead(EnterButton);
    if (EnterState == HIGH){   
        voiceRecorder.Enter();
    }
   
    int BackState = digitalRead(BackButton);
    if (BackState == HIGH){       
        voiceRecorder.Back();
    }
}
