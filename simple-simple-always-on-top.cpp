/*#include <iostream>
#include <Windows.h>


void SetWindowAlwaysOnTop(HWND hwnd) {
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void RemoveWindowAlwaysOnTop(HWND hwnd) {
    SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

HWND GetActiveWindowHandle() {
    return GetForegroundWindow();
}


int main() {
    bool alwaysOnTopActivated = false;
     
    while (true) {
        if ((GetAsyncKeyState(VK_LWIN) & 0x8000) && (GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState('T') & 0x8000)) {
            HWND hwnd = GetActiveWindowHandle();
            if (hwnd != NULL) {
                SetWindowAlwaysOnTop(hwnd);
                alwaysOnTopActivated = true;
                std::cout << "Window set as Always On Top." << std::endl;
            } else {
                std::cout << "Unable to get active window handle." << std::endl;
            }
        }

        if ((GetAsyncKeyState(VK_LWIN) & 0x8000) && (GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState('Y') & 0x8000)) {
            HWND hwnd = GetActiveWindowHandle();
            if (hwnd != NULL) {
                RemoveWindowAlwaysOnTop(hwnd);
                alwaysOnTopActivated = false;
                std::cout << "Always On Top mode disabled." << std::endl;
            } else {
                std::cout << "Unable to get active window handle." << std::endl;
            }
        }
        

        Sleep(100);
    }

    return 0;
}
*/
