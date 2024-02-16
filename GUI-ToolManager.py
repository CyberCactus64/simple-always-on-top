import os
from pystray import MenuItem as item
from pystray import Icon as setIcon
from PIL import Image
import subprocess
import ctypes


icon_path = os.path.join("graphics", "icon.png")
icon_image = Image.open(icon_path)
icon = setIcon("SimpleAlwaysOnTop", icon_image, "Simple Always On Top") # generate the icon

def run_management_app():
    subprocess.run(["path_to_the_management_app.exe"])

menu = (item("Open Management App", run_management_app),
        item("Exit", lambda: icon.stop()))

icon.menu = menu

# hide the app from the applications bar
ctypes.windll.user32.ShowWindow(ctypes.windll.kernel32.GetConsoleWindow(), 0)

icon.run()

