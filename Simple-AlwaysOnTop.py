import win32gui # pip install pywin32
import time
import ctypes

# Defining constants for window attribute modification
HWND_TOPMOST = -1
HWND_NOTOPMOST = -2
SWP_NOMOVE = 0x0002
SWP_NOSIZE = 0x0001
SWP_SHOWWINDOW = 0x0040

def set_window_always_on_top(hwnd):
    # Set the window as topmost
    win32gui.SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW)

def remove_window_always_on_top(hwnd):
    # Remove the window from Always on Top mode
    win32gui.SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW)

def get_active_window_handle():
    try:
        return win32gui.GetForegroundWindow()
    except:
        return None

def get_active_window_title():
    hwnd = get_active_window_handle()
    if hwnd:
        return win32gui.GetWindowText(hwnd)
    else:
        return None

def main():
    # Variable to keep track of the Always on Top mode activation state
    always_on_top_activated = False

    while True:
        active_window_title = get_active_window_title()
        print("Active window:", active_window_title)

        # Check if the WIN + CTRL + A key combination is pressed
        if (ctypes.windll.user32.GetKeyState(0x5B) & 0x8000) and \
           (ctypes.windll.user32.GetKeyState(0x11) & 0x8000) and \
           (ctypes.windll.user32.GetKeyState(ord('A')) & 0x8000):
            hwnd = get_active_window_handle()
            if hwnd:
                if always_on_top_activated:
                    remove_window_always_on_top(hwnd)
                    always_on_top_activated = False
                    print("Always on Top mode disabled.")
                else:
                    set_window_always_on_top(hwnd)
                    always_on_top_activated = True
                    print("Window set as Always on Top.")
            else:
                print("Unable to get active window handle.")
        else:
            always_on_top_activated = False

        time.sleep(0.1)

if __name__ == "__main__":
    main()