#include <Windows.h>
#include <shellapi.h>
#include <iostream>

#define IDM_EXIT 1001 // Definizione di IDM_EXIT come costante intera

// Dichiarazione della finestra nascosta
HWND g_hWnd = nullptr;
NOTIFYICONDATA g_nid = {};

// Funzione per creare il menu contestuale
void CreateTrayMenu(HWND hwnd) {
    HMENU hMenu = CreatePopupMenu();
    AppendMenu(hMenu, MF_STRING, IDM_EXIT, "Exit");

    POINT pt;
    GetCursorPos(&pt);
    SetForegroundWindow(hwnd);
    TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
    PostMessage(hwnd, WM_NULL, 0, 0);
}

// Funzione per gestire il menu contestuale
void HandleTrayMenu(HWND hwnd, WPARAM wParam) {
    switch (LOWORD(wParam)) {
        case IDM_EXIT:
            DestroyWindow(hwnd);
            break;
    }
}

// Finestra di callback per gestire gli eventi dell'icona
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            g_nid.cbSize = sizeof(NOTIFYICONDATA);
            g_nid.hWnd = hwnd;
            g_nid.uID = 1;
            g_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            g_nid.uCallbackMessage = WM_USER + 1;

            // Carica l'icona dalla risorsa
            g_nid.hIcon = (HICON)LoadImage(NULL, "traybar_icon.png", IMAGE_ICON, 0, 0, LR_LOADFROMFILE);
            if (g_nid.hIcon) {
                Shell_NotifyIcon(NIM_ADD, &g_nid);
            } else {
                std::cerr << "Impossibile caricare l'icona dalla risorsa." << std::endl;
            }
            break;
        case WM_USER + 1:
            switch (lParam) {
                case WM_RBUTTONDOWN:
                case WM_CONTEXTMENU:
                    CreateTrayMenu(hwnd);
                    break;
            }
            break;
        case WM_COMMAND:
            HandleTrayMenu(hwnd, wParam);
            break;
        case WM_DESTROY:
            Shell_NotifyIcon(NIM_DELETE, &g_nid);
            PostQuitMessage(0);
            break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

int main() {
    // Registra la classe della finestra
    WNDCLASS wc = {};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = GetModuleHandle(nullptr);
    wc.lpszClassName = "TrayWindowClass";
    RegisterClass(&wc);

    // Crea la finestra nascosta
    g_hWnd = CreateWindow(wc.lpszClassName, "Tray Window", 0, 0, 0, 0, 0, nullptr, nullptr, wc.hInstance, nullptr);
    if (!g_hWnd) {
        std::cerr << "Errore nella creazione della finestra." << std::endl;
        return 1;
    }

    // Esegui il ciclo dei messaggi
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
