#include <VoiceRecorder.h>

const int UpButton =14;
const int DownButton = 15;
const int EnterButton = 18;
const int BackButton = 17;


const int Position = 1;
const int FileIndex = 0;

VoiceRecorder VR(UpButton, DownButton, EnterButton, BackButton, Position, FileIndex);

void setup() {
    VR.Start();
}

void loop(){
    int UpState = digitalRead(UpButton);
    if (UpState == HIGH){       
        VR.Up();
    }

    int DownState = digitalRead(DownButton);
    if (DownState == HIGH){      
        VR.Down();
    }

    int EnterState = digitalRead(EnterButton);
    if (EnterState == HIGH){   
        VR.Enter();
    }
   
    int BackState = digitalRead(BackButton);
    if (BackState == HIGH){       
        VR.Back();
    }
}
