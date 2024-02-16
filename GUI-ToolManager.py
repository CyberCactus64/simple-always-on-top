import pystray
from pystray import MenuItem as item
from PIL import Image
import subprocess

# create the icon in the traybar
icon = pystray.Icon("icon", title="Simple Always On Top")
icon.icon = Image.open("icons/icon.png")

def run_exe():
    subprocess.run(["path_to_your_executable_file.exe"])

# right click menu
menu = (item("Open Management App", run_exe),
        item("Exit", lambda: icon.stop()))

icon.menu = menu

icon.run()