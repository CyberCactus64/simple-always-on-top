import os
from pystray import MenuItem as item
from pystray import Icon as setIcon
from PIL import Image
import subprocess
import ctypes


icon_path = os.path.join("icons", "icon.png") # /folder
icon_image = Image.open(icon_path)
icon = setIcon("SimpleAlwaysOnTop", icon_image, "Simple Always On Top") # generate the icon

def run_management_app():
    subprocess.run(["path_to_your_management_app.exe"])

# Definizione delle azioni del menu contestuale
menu = (item("Open Management App", run_management_app),
        item("Exit", lambda: icon.stop()))

# Associazione del menu contestuale all'icona della traybar
icon.menu = menu

# Nascondi l'icona dell'applicazione dalla barra delle applicazioni
ctypes.windll.user32.ShowWindow(ctypes.windll.kernel32.GetConsoleWindow(), 0)

# Avvio dell'applicazione
icon.run()

