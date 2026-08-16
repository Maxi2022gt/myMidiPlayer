// I need to clean up this code, fr..
// honestly I wonder how long will it take for someone to rip the midi from this, even though it's procedurally generated


#include <iostream>
#include <windows.h>

#pragma comment(lib,"winmm.lib")

int c = 0;
int q = 0;

DWORD midiMsg(BYTE status, BYTE data1, BYTE data2)
{
    return status | (data1 << 8) | (data2 << 16);
}

HMIDIOUT hMidiOut;



DWORD WINAPI Note(LPVOID lpVoid) {
    c++;
    midiOutShortMsg(hMidiOut, midiMsg(0x90, 60 + ((c * (1+q)) % 12), 127));
    Sleep(1300);
    midiOutShortMsg(hMidiOut, midiMsg(0x80, 60 + ((c * (1 + q)) % 12), 127));
    Sleep(1300);

    return 0;
}


int main()
{
    int whatDevice = 0;
    UINT numDevices = midiOutGetNumDevs();

    std::cout << "MIDI outputs: " << numDevices << "\n";

    for (UINT i = 0; i < numDevices; i++)
    {
        MIDIOUTCAPS caps{};

        midiOutGetDevCaps(i, &caps, sizeof(caps));

        std::wcout << i << L": "
            << caps.szPname
            << L"\n";
    }

    bool lastUp = false;
    bool lastDown = false;
    bool lastEnter = false;
    
    while (true) {
        whatDevice %= numDevices;
        if (whatDevice < 0) whatDevice = numDevices - 1;

        MIDIOUTCAPS caps{};

        midiOutGetDevCaps(whatDevice, &caps, sizeof(caps));
        bool up = GetAsyncKeyState(VK_UP) & 0x8000;
        bool down = GetAsyncKeyState(VK_DOWN) & 0x8000;
        bool enter = GetAsyncKeyState(VK_RETURN) & 0x8000;

        if (up && !lastUp) {
            whatDevice++;
        }

        if (down && !lastDown) {
            whatDevice--;
        }

        if (enter && !lastEnter) {
            break;
        }
        lastUp = up;
        lastDown = down;
        lastEnter = enter;

        std::cout << "\033[" << (numDevices - 1) << "0;0H";
        std::wcout << L"Choose what device to use: " << whatDevice << L" (" << caps.szPname << L")                                     ";

        Sleep(20);
    }

    std::cout << std::endl;

    MMRESULT result = midiOutOpen(&hMidiOut, whatDevice, 0, 0, CALLBACK_NULL);

    if (result != MMSYSERR_NOERROR)
    {
        std::cout << "midiOutOpen failed: " << result << "\n";
        return 1;
    }

    for (int i2 = 0; i2 < 11; i2++) {
        for (int i = 0; i < 20; i++) {
            std::cout << "boop";
            HANDLE h = CreateThread(NULL, 0, Note, NULL, NULL, NULL);
            WaitForSingleObject(h, 20);
            Sleep(200);
        }
        q++;
    }

    for (int i2 = 1; i2 < 50; i2++) {
        for (int i = 0; i < 127; i++) {
            std::cout << "BEEEP";
            midiOutShortMsg(hMidiOut, midiMsg(0x90, i, 127));
            Sleep((50/i2));
        }
        for (int i = 0; i < 127; i++) {
            midiOutShortMsg(hMidiOut, midiMsg(0x80, i, 127));
        }
    }

    for (int _ = 0; _ < 120; _++) {
        std::cout << "uisdlfhjaiodfoumasdof";
        for (int i = 0; i < 127; i++) {
            midiOutShortMsg(hMidiOut, midiMsg(0x90, i, 127));
        }
        Sleep(1);
        for (int i = 0; i < 127; i++) {
            midiOutShortMsg(hMidiOut, midiMsg(0x80, i, 127));
        }
    }

    for (int i = 0; i < 127; i++) {
            std::cout << std::endl;
            std::cout << "Boom";
            midiOutShortMsg(hMidiOut, midiMsg(0x90, i, 127));
    }

    Sleep(5000);

    for (int i = 0; i < 127; i++) {
        midiOutShortMsg(hMidiOut, midiMsg(0x80, i, 127));
    }

    Sleep(2000);

    for (int i = 0; i < 127; i++) {
        std::cout << std::endl;
    }

    std::cout << "I was bored :P";

    midiOutShortMsg(hMidiOut, midiMsg(0x90, 79, 127));

    Sleep(100);

    midiOutShortMsg(hMidiOut, midiMsg(0x80, 79, 127));

    Sleep(3000);

    midiOutClose(hMidiOut);
}
