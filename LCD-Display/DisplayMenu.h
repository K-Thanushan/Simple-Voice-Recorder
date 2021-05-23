#ifndef DisplayMenu_h
#define DisplayMenu_h

#include "Arduino.h"


class DisplayMenu
{
    public:
        DisplayMenu(int UpButton, int DownButton, int EnterButton, int BackButton, int Position, int FileIndex);
        void Start();
        void Up();
        void Down();
        void Enter();
        void Back();
        void MenuPosition1(int a, String Displaytext1, String Displaytext2);
        void MenuPosition2(int a, String Displaytext3, String Displaytext4);
        String *GetFileList();
        int _enter;
        int _back;
        int _down;
        int _up;
        int _Position;
        int _FileIndex; 
        void startRecording();
        void stopRecording();
        void startPlayback(String fileName);
        void deleteRecording(String fileName);

};

#endif;