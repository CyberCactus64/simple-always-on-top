'''
 - IF YOU WANT TO WRITE AND TEST NEW FUNCTIONS, FEEL FREE TO MODIFY THIS SCRIPT :)
 - This is a sort of a "beta" version for every new Release
'''


import win32gui # pip install pywin32
import time
import ctypes


# Constants for window attribute modification
HWND_TOPMOST = -1
HWND_NOTOPMOST = -2
SWP_NOMOVE = 0x0002
SWP_NOSIZE = 0x0001
SWP_SHOWWINDOW = 0x0040


def set_window_always_on_top(hwnd):
    win32gui.SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW)

def remove_window_always_on_top(hwnd):
    win32gui.SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW)

def get_active_window_handle():
    try:
        return win32gui.GetForegroundWindow()
    except:
        return None
    
# UNCOMMENT TO TEST (can be called to print the active window name)
''' def get_active_window_title():
    hwnd = get_active_window_handle()
    if get_active_window_handle():
        return win32gui.GetWindowText(hwnd)
    else:
        return None '''

def main():
    always_on_top_activated = False

    while True:
        # UNCOMMENT TO TEST:
        # print("Active window:", get_active_window_title())

        # Check if the WIN + SHIFT + T key combination is pressed
        if (ctypes.windll.user32.GetKeyState(0x5B) & 0x8000) and \
           (ctypes.windll.user32.GetKeyState(0x10) & 0x8000) and \
           (ctypes.windll.user32.GetKeyState(ord('T')) & 0x8000):
            hwnd = get_active_window_handle()
            if hwnd:
                if always_on_top_activated:
                    remove_window_always_on_top(hwnd)
                    always_on_top_activated = False
                    print("Always on Top mode disabled.")
                else: # default: always on top non activated
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