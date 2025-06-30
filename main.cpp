#include "Logging.h"
#include "ConsoleAnimation.h"
#include "D3DMenu.h"
#include "Hooking.h"
#include "GameHacks.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        CreateThread(nullptr, 0, InitThread, nullptr, 0, nullptr);
    }
    else if (reason == DLL_PROCESS_DETACH) {
        g_Menu.Release();
        ToggleESPNameTag(false);
        ToggleAntiKick(false);
        ToggleNoRecoil(false);
        MH_DisableHook(MH_ALL_HOOKS);
        MH_Uninitialize();
        Log("DLL unloaded.");
        FreeConsole();
    }
    return TRUE;
}