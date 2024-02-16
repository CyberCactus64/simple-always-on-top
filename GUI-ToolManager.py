import os
import pystray
from pystray import MenuItem as item
from PIL import Image
import subprocess
import ctypes

# Costruisci il percorso completo dell'icona
icon_path = os.path.join("icons", "icon.png")

# Carica l'immagine .png per l'icona della traybar
icon_image = Image.open(icon_path)

# Creazione dell'icona della traybar
icon = pystray.Icon("example_icon", icon_image, "Example Icon")

# Funzione per avviare l'applicazione di gestione
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

