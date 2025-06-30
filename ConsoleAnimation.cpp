#include <windows.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "ConsoleAnimation.h"
#include "Logging.h"

const char* titlePrefixes[] = {
    "Intercept"
};
const int prefixCount = sizeof(titlePrefixes) / sizeof(titlePrefixes[0]);

void ShowConsoleAnimation() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    printf(ANSI_BOLD ANSI_MAGENTA "\n=== Intercept: Simple Menu V1 ===\n" ANSI_RESET);
    printf(ANSI_CYAN "Initializing...\n" ANSI_RESET);

    const char* spinner = "|/-\\";
    for (int i = 0; i < 20; i++) {
        printf("\r" ANSI_YELLOW "Loading %c" ANSI_RESET, spinner[i % 4]);
        fflush(stdout);
        Sleep(100);
    }
    printf("\r" ANSI_GREEN "Initialization Complete!      \n" ANSI_RESET);
    printf(ANSI_BOLD ANSI_BLUE "Press INSERT to toggle menu\n" ANSI_RESET);
    printf(ANSI_CYAN "----------------------------\n" ANSI_RESET);
}

DWORD WINAPI AnimatedTitleThread(LPVOID) {
    const char* spinner = "|/-\\";
    char title[256];
    srand((unsigned int)time(nullptr));
    int prefixIndex = rand() % prefixCount;
    const char* baseTitle = titlePrefixes[prefixIndex];

    while (true) {
        for (int i = 0; i < 4; i++) {
            snprintf(title, sizeof(title), "%s: Simple Menu V1 %c", baseTitle, spinner[i]);
            SetConsoleTitleA(title);
            Sleep(200);
        }
    }
    return 0;
}