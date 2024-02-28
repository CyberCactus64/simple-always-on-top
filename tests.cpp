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

int main() {
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

    // Esegui il ciclo dei messaggi
    MSG msg;
    BOOL bRet;
    while ((bRet = GetMessage(&msg, nullptr, 0, 0)) != 0) {
        if (bRet == -1) {
            // Errore durante la ricezione del messaggio
            std::cerr << "Errore durante la ricezione del messaggio." << std::endl;
            break;
        } else {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return 0;
}

