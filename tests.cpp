#include <Windows.h>
#include <shellapi.h>
#include <iostream>


// tray bar menu options identifiers
#define IDM_EXIT 1001
#define IDM_OPEN_TOOL_MANAGER 1002

// hidden window:
HWND g_hWnd = nullptr;
NOTIFYICONDATA icon_data = {};

// this function creates the popup menu from the tray bar menu icon
void CreateTrayMenu(HWND hwnd) {
    HMENU popupMenu = CreatePopupMenu();
    AppendMenu(popupMenu, MF_STRING, IDM_EXIT, "Exit"); // create "Exit" option
    AppendMenu(popupMenu, MF_STRING, IDM_OPEN_TOOL_MANAGER, "Open Tool Manager"); // create "Open Tool Manager" option

    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(hwnd);
    TrackPopupMenu(popupMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
    PostMessage(hwnd, WM_NULL, 0, 0);
}

// this function assigns actions to tray bar menu options 
void HandleTrayMenu(HWND hwnd, WPARAM wParam) {
    switch (LOWORD(wParam)) {
        case IDM_EXIT:
            DestroyWindow(hwnd);
            break;
        case IDM_OPEN_TOOL_MANAGER:
            std::cout << "...TOOL MANAGER WILL BE ADDED AS SOON AS POSSIBLE... :)\n" << std::endl;
    }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            icon_data.cbSize = sizeof(NOTIFYICONDATA);
            icon_data.hWnd = hwnd;
            icon_data.uID = 1;
            icon_data.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            icon_data.uCallbackMessage = WM_USER + 1;

            // load the icon from the folder /graphics
            icon_data.hIcon = (HICON)LoadImage(NULL, "graphics\\traybar_icon.ico", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
            if (!Shell_NotifyIcon(NIM_ADD, &icon_data)) {
                DWORD error = GetLastError();
                std::cerr << "Error while loading the tray bar menu icon: " << error << std::endl;
            }
            break;
        case WM_USER + 1:
            switch (lParam) {
                case WM_RBUTTONDOWN:
                case WM_CONTEXTMENU:
                    CreateTrayMenu(hwnd); // call the function to create the tray menu icon
                    break;
            }
            break;
        case WM_COMMAND:
            HandleTrayMenu(hwnd, wParam); // call the function to handle actions from the tray menu icon
            break;
        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, &icon_data);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}


void SetWindowAlwaysOnTop(HWND hwnd) {
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void RemoveWindowAlwaysOnTop(HWND hwnd) {
    SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}


// background function of the thread
DWORD WINAPI BackgroundThread(LPVOID lpParam) {
    bool alwaysOnTopActivated = false;

    while (true) {
        // background code
        if ((GetAsyncKeyState(VK_LWIN) & 0x8000) && (GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState('T') & 0x8000)) { // win + shift + t
            HWND hwnd = GetForegroundWindow();
            if (hwnd != NULL) {
                SetWindowAlwaysOnTop(hwnd);
                alwaysOnTopActivated = true;
                std::cout << "Window set as Always On Top." << std::endl;
            } else {
                std::cout << "Unable to get active window handle." << std::endl;
            }
        }

        if ((GetAsyncKeyState(VK_LWIN) & 0x8000) && (GetAsyncKeyState(VK_SHIFT) & 0x8000) && (GetAsyncKeyState('Y') & 0x8000)) { // win + shift + y
            HWND hwnd = GetForegroundWindow();
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
    HANDLE hThread; // to manage the thread

    // create the thread in background (call the function BackgroundThread())
    hThread = CreateThread(NULL, 0, BackgroundThread, NULL, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "Error...." << GetLastError() << std::endl;
        return 1;
    }
    std::cout << "HOW TO USE:\nENABLE Always On Top mode --> WIN + SHIFT + T\nDISABLE Always On Top mode --> WIN + SHIFT + Y\n" << std::endl;
    
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = "TrayWindowClass";
    RegisterClass(&wc);

    // TRYING TO FINISH THE INVISIBLE WINDOW... IT DOESN'T WORKS :')
    g_hWnd = CreateWindow(wc.lpszClassName, "Tray Window", 0, 0, 0, 0, 0, nullptr, nullptr, wc.hInstance, nullptr);
    if (!g_hWnd) {
        std::cerr << "Unable to create the window..." << std::endl;
        return 1;
    }

    MSG msg;
    BOOL bRet;
    while ((bRet = GetMessage(&msg, nullptr, 0, 0)) != 0) {
        if (bRet == -1) {
            std::cerr << "Errore durante la ricezione del messaggio." << std::endl;
            break;
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}