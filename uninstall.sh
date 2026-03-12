#!/bin/bash
# Script de désinstallation ultra-discret

# Chemins cachés
HIDDEN_DIR="$HOME/.local/share/.sys-updates"
BIN_NAME=".dbus-helper"
DESKTOP_FILE="$HOME/.config/autostart/dbus-helper.desktop"

echo "[*] Arrêt du service caché..."
killall "$BIN_NAME" 2>/dev/null

echo "[*] Suppression de l'auto-démarrage..."
rm -f "$DESKTOP_FILE"

echo "[*] Suppression des fichiers système..."
rm -rf "$HIDDEN_DIR"

echo "[+] Le système est parfaitement propre."
