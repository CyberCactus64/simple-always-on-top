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

// Funzione che verrà eseguita in background
DWORD WINAPI BackgroundThread(LPVOID lpParam) {
    bool alwaysOnTopActivated = false;

    std::cout << "Codice in background in esecuzione..." << std::endl;

    while (true) {
        // Codice da eseguire in background...
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
    HANDLE hThread; // variabile per gestire il thread

    // Crea il thread in background (richiama funzione BackgroundThread() sopra)
    hThread = CreateThread(NULL, 0, BackgroundThread, NULL, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "Errore durante la creazione del thread: " << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "Thread in background creato con successo!" << std::endl;

    // attende che il thread venga chiuso
    WaitForSingleObject(hThread, INFINITE);

    // chiude l'handle del thread
    CloseHandle(hThread);

    return 0;
}
