#include "Logging.h"

void Log(const char* fmt, ...) {
    char buf[512];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);

    OutputDebugStringA("[D3DMenu] ");
    OutputDebugStringA(buf);
    OutputDebugStringA("\n");

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    std::string message = std::string(ANSI_CYAN) + "[D3DMenu] " + ANSI_RESET + buf + "\n";

    if (strstr(buf, "failed") || strstr(buf, "Failed")) {
        message = std::string(ANSI_RED) + "[D3DMenu] " + ANSI_RESET + buf + "\n";
    }
    else if (strstr(buf, "enabled") || strstr(buf, "success") || strstr(buf, "initialized")) {
        message = std::string(ANSI_GREEN) + "[D3DMenu] " + ANSI_RESET + buf + "\n";
    }
    else if (strstr(buf, "retrying") || strstr(buf, "Retrying")) {
        message = std::string(ANSI_YELLOW) + "[D3DMenu] " + ANSI_RESET + buf + "\n";
    }

    printf("%s", message.c_str());
    fflush(stdout);
}