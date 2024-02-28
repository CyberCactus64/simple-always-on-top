#include <Windows.h>
#include <iostream>


void SetWindowAlwaysOnTop(HWND hwnd) {
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void RemoveWindowAlwaysOnTop(HWND hwnd) {
    SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

HWND GetActiveWindowHandle() {
    return GetForegroundWindow();
}

// background function of the thread
DWORD WINAPI BackgroundThread(LPVOID lpParam) {
    bool alwaysOnTopActivated = false;

    while (true) {
        // background code:
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

int main() {
    HANDLE hThread; // manage the thread

    // create the thread in background (call the function BackgroundThread())
    hThread = CreateThread(NULL, 0, BackgroundThread, NULL, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "Error...." << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "HOW TO USE:\nENABLE Always On Top mode --> WIN + SHIFT + T\nDISABLE Always On Top mode --> WIN + SHIFT + Y\n" << std::endl;

    // wait while thread is running
    WaitForSingleObject(hThread, INFINITE);

    // close the thread
    CloseHandle(hThread);

    return 0;
}
